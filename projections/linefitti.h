void keyboardManager( unsigned char key, int x, int y );

void mouseManager( int button, int state, int x, int y );
void mouseMovedPressed( int x, int y );
void mouseMoved( int x, int y );


void displayLines(void);
void removeAllPoints();
void addNewPoint( float x, float y, int state );

void initRendering();
void resizeWindow(int w, int h);