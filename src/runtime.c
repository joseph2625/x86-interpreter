#include "runtime.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "pthread.h"
#include "interpreter.h"

void update_virtual_memory_lookup_table_on_create( RuntimeEnvironment_t *environment, VirtualPageNode_t *newly_created_page ) {
  uint32_t address = newly_created_page->begin;
  unsigned char *buffer = newly_created_page->buffer;

  do{
    VirtualPageLookupTable_t *pageLookupTable = environment->directory_table.page_lookup_table[address >> 22];
    if( pageLookupTable == NULL ) {
      pageLookupTable = environment->directory_table.page_lookup_table[address >> 22] = (VirtualPageLookupTable_t *)calloc( 1, sizeof(VirtualPageLookupTable_t) );
      if( pageLookupTable == NULL )
        assert( 0 );
    }
    if( pageLookupTable->frames[ (address & 0x3FF000) >> 12 ] != NULL )
      assert( 0 );
    pageLookupTable->frames[ (address & 0x3FF000) >> 12 ] = buffer;

    buffer += 4096;
    address += 4096;
  } while( address < newly_created_page->begin + newly_created_page->size );
  

}

uint32_t insert_new_page_in_between( VirtualPageNode_t * prev, uint32_t requested_base, uint32_t requested_size, unsigned char * initial_data, uint32_t initial_data_size, VirtualPageNode_t * next, RuntimeEnvironment_t * environment ) 
{
  prev->next = (VirtualPageNode_t *)malloc( sizeof( VirtualPageNode_t ) );

  if( prev->next == NULL )
    return 0;

  prev->next->begin = requested_base == UNSPECIFIED_BASE ? prev->begin + prev->size : requested_base;
  prev->next->buffer = (unsigned char *)calloc( requested_size, 1 );

  if( prev->next->buffer == NULL )
    return 0;

  if( initial_data != NULL )
    memcpy( prev->next->buffer, initial_data, initial_data_size );

  prev->next->size = requested_size;
  prev->next->next = next;

  update_virtual_memory_lookup_table_on_create( environment, prev->next );
  return prev->next->begin;
}
uint32_t create_virtual_memory_page( RuntimeEnvironment_t *environment, uint32_t requested_size, uint32_t requested_base, unsigned char *initial_data, uint32_t initial_data_size){

  VirtualPageNode_t *next = environment->page_list->next;
  VirtualPageNode_t *prev = environment->page_list;

  if( requested_size == 0 )
    return 0;

  //requested_size must be a multiple of 4KB
  if( requested_size != UNSPECIFIED_SIZE && requested_size % 4096 > 0 )
    return 0;

  //if initial_data exists, but the size of the data is not specified, then return false.
  if( initial_data != NULL && initial_data_size == 0 )
    return 0;

  //if initial_data does not exist, but the size of the data is specified, then return false.
  if( initial_data == NULL && initial_data_size != UNSPECIFIED_SIZE )
    return 0;

  //if requested size does not exist, but the size of the data is specified, then return false.
  if( requested_size == UNSPECIFIED_SIZE && initial_data_size != UNSPECIFIED_SIZE )
    return 0;

  if( requested_size != UNSPECIFIED_SIZE && initial_data_size != UNSPECIFIED_SIZE && initial_data_size > requested_size )
    return 0;

  if( requested_base != UNSPECIFIED_BASE && ( ( requested_base >= 0 && requested_base < 4096*10 ) || requested_base >= 0x80000000 || requested_base % 4096 > 0 ) )
    return 0;


  if( requested_size == UNSPECIFIED_SIZE )
    requested_size = 4096;

  do{
    if( requested_base == UNSPECIFIED_BASE && next->begin - prev->begin + prev->size >= requested_size ) {
      return insert_new_page_in_between(prev, requested_base, requested_size, initial_data, initial_data_size, next, environment);
    } else if( requested_base != UNSPECIFIED_BASE ){
      //TODO: Rewrite this section
      //Overlap
      if( requested_base > next->begin ) {
        if( requested_base < next->begin + next->size )
          return false;
        else
          prev = next;
          next = next->next;
          continue;
      }

      if( requested_base >= prev->begin + prev->size ) {
        //Too small
        if( requested_base + requested_size > next->begin )
          return false;
        
        //Found!
        return insert_new_page_in_between(prev, requested_base, requested_size, initial_data, initial_data_size, next, environment);
      }
    }
    prev = next;
    next = next->next;
  } while( prev->begin != 0x80000000 );
  //not enough memory
  return 0;
}
bool load_sections_to_virtual_memory( Image_t *image, RuntimeEnvironment_t *environment ){

  environment->page_list = (VirtualPageNode_t *)malloc( sizeof(VirtualPageNode_t) );
  
  if( environment->page_list == NULL )
    return false;

  environment->page_list->buffer = NULL;
  environment->page_list->begin = 0;
  environment->page_list->size = 4096 * 16;
  environment->page_list->next = (VirtualPageNode_t *)malloc( sizeof(VirtualPageNode_t) );
  
  if( environment->page_list->next == NULL )
    return false;

  environment->page_list->next->buffer = NULL;
  environment->page_list->next->begin = 0x80000000;
  environment->page_list->next->size = 0x80000000;
  environment->page_list->next->next = NULL;


  for( unsigned int i = 0; i < image->number_of_sections; i++ ) {
    if( image->sections[i].virtual_size < image->sections[i].raw_size )
      image->sections[i].virtual_size = image->sections[i].raw_size;

    if( image->sections[i].virtual_size % 4096 > 0 )
      image->sections[i].virtual_size += ( 4096 - ( image->sections[i].virtual_size % 4096 ) );

    if( !create_virtual_memory_page( environment, image->sections[i].virtual_size, image->image_base + image->sections[i].relative_virtual_base_address, image->sections[i].buffer, image->sections[i].raw_size ) )
      return false;
  }

  return true;
}
void *runtime_thread( void *context ) {
  PthreadContext_t *pthread_context = (PthreadContext_t *)context;

  pthread_context->thread_node->exit_code = interpret( pthread_context->runtime_environment, pthread_context->thread_node );

  pthread_context->thread_node->state = EXITED;

  pthread_mutex_lock( pthread_context->mutex );
  sem_post( pthread_context->notifier_sem );
  sem_wait( pthread_context->wait_sem );
  pthread_mutex_unlock( pthread_context->mutex );
  free( context );
  return NULL;
}

bool create_thread( RuntimeEnvironment_t *environment, uint32_t entry_point, uint32_t stack_size, pthread_mutex_t *mutex, sem_t *notifier_sem, sem_t *wait_sem  )
{

  ThreadNode_t *temp_node = environment->threads;

  environment->thread_count++;
  environment->threads = (ThreadNode_t *)calloc( 1, sizeof(ThreadNode_t) );
  environment->threads->next = temp_node;
  environment->threads->state = RUNNING;

  environment->threads->context.eip = 4096*16;
  environment->threads->context.eax = entry_point;
  environment->threads->context.esp = create_virtual_memory_page( environment, stack_size, UNSPECIFIED_BASE, NULL, UNSPECIFIED_SIZE );
  environment->threads->context.code = get_real_address( environment->threads->context.eip, &environment->directory_table, EXECUTE, false );
  
  if( environment->threads->context.esp == 0 ) {
    free(environment->threads);
    environment->threads = temp_node;
    return false;
  }


  environment->threads->context.esp += stack_size - 8;

  PthreadContext_t *context = (PthreadContext_t *)malloc( sizeof(PthreadContext_t) );
  context->thread_node = environment->threads;
  context->runtime_environment = environment;
  context->mutex = mutex;
  context->notifier_sem = notifier_sem;
  context->wait_sem = wait_sem;

  pthread_create(&environment->threads->pthread, NULL, runtime_thread, context);

  return true;
}

bool insert_thread_entry_point( RuntimeEnvironment_t *environment, uint32_t initial_stack_size )
{
  unsigned char wrapper_opcodes[] = { 0xFF, 0xD0, 0x8B, 0xD8, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xCD, 0x80 };//call eax; mov ebx, eax; mov eax, 1; int 80h
  if( create_virtual_memory_page(environment, 4096, 4096*16, wrapper_opcodes, sizeof(wrapper_opcodes)) == 0 )
    return false;

  return true;
}
bool set_up_runtime_environment( Image_t *image, RuntimeEnvironment_t *environment, pthread_mutex_t *mutex, sem_t *notifier_sem, sem_t *wait_sem )
{
  memset( environment, 0, sizeof(RuntimeEnvironment_t) );
  
  if( !load_sections_to_virtual_memory( image, environment ) )
    return false;

  if( !insert_thread_entry_point( environment, image->stack_size ) )
    return false;

  return create_thread( environment, image->image_base + image->relative_virtual_entry_point, image->stack_size, mutex, notifier_sem, wait_sem );
}
bool update_runtime_environment( RuntimeEnvironment_t *environment ){
  if( environment->threads == NULL )
    assert( 0 );

  ThreadNode_t *thread = environment->threads;
  ThreadNode_t **prev = &environment->threads;
  ThreadNode_t *to_delete;

  while( thread != NULL ){
    if( thread->state == EXITED ) {
      (*prev) = thread->next;
      to_delete = thread;
      thread = thread->next;
      free( to_delete );
    } else {
      prev = &thread->next;
      thread = thread->next;
    }
  }

  if( environment->threads == NULL )
    return false;

  return true;
}
