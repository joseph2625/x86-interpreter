#ifdef _MSC_VER //as per Microsoft KB 99261 (http://support.microsoft.com/kb/99261)

#include <windows.h>


void cls( HANDLE hConsole )
{
  COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                      cursor */ 
  BOOL bSuccess;
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
  DWORD dwConSize;                 /* number of character cells in
                                      the current buffer */ 

  /* get the number of character cells in the current buffer */ 

  bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
  
  if( !bSuccess )
    return;

  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

  /* fill the entire screen with blanks */ 

  bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
      dwConSize, coordScreen, &cCharsWritten );

  if( !bSuccess )
    return;

  /* get the current text attribute */ 

  bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
  if( !bSuccess )
    return;

  /* now set the buffer's attributes accordingly */ 

  bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
      dwConSize, coordScreen, &cCharsWritten );
  if( !bSuccess )
    return;

  /* put the cursor at (0, 0) */ 

  bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
  if( !bSuccess )
    return;
  return;
}

void clear_stdout() {
  cls( GetStdHandle(STD_OUTPUT_HANDLE) );
}

#endif