#include "syscall.h"

size_t handle_sys_write( VirtualDirectoryLookupTable_t *table, int fd, uint32_t buffer_address, size_t count ) {
  if(fd == 1) { //ONLY SUPPORTS STDOUT FOR NOW
    //TODO: IMPLEMENT PROPER BUFFER ACCESS CHECK
    unsigned char *buffer = get_real_address( buffer_address, table, READ, true );
    if( buffer != NULL ) {
      if( fwrite( buffer, 1, count, stdout ) != count ) //THIS DOES NOT APPLY TO OTHER FDs
        return -EIO;
      else
        return count;
    } else {
      return -EFAULT;
    }
  } else {
    return -EBADF;
  }
}

size_t handle_sys_read( VirtualDirectoryLookupTable_t *table, int fd, uint32_t buffer_address, size_t count ) {
  if(fd == 0) { //ONLY SUPPORTS STDIN FOR NOW
    //TODO: IMPLEMENT PROPER BUFFER ACCESS CHECK
    unsigned char *buffer = get_real_address( buffer_address, table, WRITE, true );
    if( buffer != NULL ) {
      if( fread( buffer, 1, count, stdin ) != count ) //THIS DOES NOT APPLY TO OTHER FDs
        return -EIO;
      else
        return count;
    } else {
      return -EFAULT;
    }
  } else {
    return -EBADF;
  }
}

int handle_sys_clock_gettime( VirtualDirectoryLookupTable_t *table, clockid_t clkid, uint32_t timespec_address ){
  if( clkid == CLOCK_MONOTONIC ) {
    timespec_t *timespec = (timespec_t *)get_real_address(timespec_address, table, READ, true);
    if( timespec ) {
#ifdef _WIN32
      //timespec->tv_sec = GetTickCount();
      
      LARGE_INTEGER count;
      LARGE_INTEGER freq;
      if( QueryPerformanceCounter( &count ) && QueryPerformanceFrequency( &freq ) ) {
        timespec->tv_nsec = (count.QuadPart * 1000000000ULL) / freq.QuadPart;
        timespec->tv_sec = count.QuadPart / freq.QuadPart;
        return 0;
      } else {
        return -EPERM;
      }
#else
      assert(0);
#endif
    } else {
      return -EFAULT;
    }
  } else {
    return -EINVAL;
  }
}