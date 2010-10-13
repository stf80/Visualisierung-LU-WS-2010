// main.cpp : Defines the entry point for the console application.
//

/****************************************************************************************************
	VisLU	-	Institute of Computergraphics and Algorithms	- Vienna University of Technology	


This project contains a simple framework that implements some basic math classes 
such as Vector, Matrix, Quaternion, etc. and some further classes that are of help
to implement a basic volume rendering approach.

The file main.cpp demonstrates the use of the (pre-)implemented classes.
For the VisLU it is necessarry to implement further classes for transfer functions,
user interface, etc. and to replace the implementation of the main.cpp file.
Currently, the main.cpp file loads a dataset (dat-file) and tries to upload it as a 
3D texture to the graphics card. 
If the graphics card does not support 3D textures, one slice of the volume is uploaded as 2D texture.
In the display method a slice is renderered. 
If 3D textures are supported the slice is altered in the idle-function otherwise just the static 2D texture is shown.

The main.cpp file further demonstrates how to take screen-shots, how to parse command line arguments, 
how to make use of glut and glew, and how to use a timer.


*****************************************************************************************************/

#include "common.h"
#include "Timer.h"
#include "Arguments.h"
#include "Volume.h"

#include <windows.h>
#include <stdlib.h>
#include <io.h>
#include <glew.h>
#include <openglut.h>
#include <il.h>
#include <math.h>


// Global variables
int g_iWidth = 768, g_iHeight = 768; // window dimensions
Arguments g_argArguments; // arguments that are parsed from the console
Volume g_Volume; // our volume
unsigned int g_uExampleTexture = 0; // texture
float g_fCoordZ = 0.5f; // z-Coordinate of the currently displayed slice (only for 3D textures)
float g_fIncrement = 0.f; // increment for the z-texture coordinates (only for 3D textures)

// Our glutIdleFunc
void glutIdle(void)
{
	
	if (g_Volume.GetDepth()>1)
	{
		//update the slice z-coordinate
		g_fCoordZ += g_fIncrement;
		if (g_fCoordZ > 1.f)
		{
			g_fIncrement*=-1.f;
			g_fCoordZ = 1.f;
		}
		else if (g_fCoordZ<0.f)
		{
			g_fIncrement *=-1.f;
			g_fCoordZ = 0.f;
		}
	}

	//trigger the display function (glutDisplay is called in the next iteration of the glutMainLoop)
	glutPostRedisplay();
}

// Our glutDisplayFunc
void glutDisplay(void)
{
	static Timer timTimer;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();


	//todo: implement rendering here

	
	if (g_uExampleTexture) //texture is created
	{
		if(GLEW_EXT_texture3D) //graphics hw supports 3d textures
		{
			glEnable(GL_TEXTURE_3D);
			//draw quad with texture taken from 3d volume
			glBegin(GL_QUADS);
			glTexCoord3f(0.0f, 0.0f, g_fCoordZ);
			glVertex3f(-0.5,-0.5,-1);
			glTexCoord3f(1.0f, 0.0f, g_fCoordZ);
			glVertex3f(0.5,-0.5,-1);
			glTexCoord3f(1.0f, 1.0f, g_fCoordZ);
			glVertex3f(0.5,0.5,-1);
			glTexCoord3f(0.0f, 1.0f, g_fCoordZ);
			glVertex3f(-0.5,0.5,-1);
			glEnd();
		}
		else // 3d texture not supported
		{
			glEnable(GL_TEXTURE_2D);
			//draw quad with 2d texture
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-0.5,-0.5,-1);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5,-0.5,-1);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5,0.5,-1);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-0.5,0.5,-1);
			glEnd();
		}
	}

	glPopMatrix();
	glutSwapBuffers();

	//estimate fps
	const double dTimePerFrame = double(timTimer);
	const double dFramesPerSecond = dTimePerFrame > 0.0 ? 1.0 / dTimePerFrame : 0.0;
	timTimer.start();
	printf("- %06.2f frames/second \r",dFramesPerSecond);

}

// Our glutReshapeFunc
void glutReshape(int width, int height)
{
	g_iWidth = width;
	g_iHeight = height;
	
	//todo: reshape rendering viewport here
}

// Our glutMouseFunc
void glutMouse(int button, int state, int x, int y)
{
	//todo: do mouse interaction here
}

// Our glutMotionFunc
void glutMotion(int x, int y)
{
}



// Our glutKeyboardFunc
void glutKeyboard(unsigned char key, int x, int y)
{
	//todo: do keyboard interaction here

	//this saves a screenshot of the OpenGL window if Ctrl+G is pressed
	if (key == unsigned char(7) && (glutGetModifiers() & GLUT_ACTIVE_CTRL)) // Ctrl+G to save screenshots
	{		
		unsigned int uImage = 0;
		std::vector<unsigned char> vecImage;

		char vcFilename[1024];
		// Check for next free filename, since we don't want to overwrite existing images
		// images will be saved as 'VisLuFramework0000.png','VisLuFramework0001.png','VisLuFramework0002.png',etc.
		for (int i=0;i<9999;i++)
		{
			sprintf_s(vcFilename,1024,"VisLuFramework%04d.png",i);

			// check if file does not exist
			if( (_access(vcFilename,0)) == -1 )
				break;
		}

		std::cout << "- Saving screenshot to '" << vcFilename << "' ..." << std::endl;

		vecImage.resize(g_iWidth*g_iHeight*4);

		// Read the OpenGL framebuffer
		glPushAttrib(GL_PIXEL_MODE_BIT);
		glReadBuffer(GL_FRONT);

		glReadPixels(0,0,g_iWidth,g_iHeight,GL_RGBA,GL_UNSIGNED_BYTE,(void*) &(vecImage.front()) );
		const GLenum glError = glGetError();	
		
		glPopAttrib();

		if (glError != GL_NO_ERROR)
		{
			std::cerr<<"Unable to read frame buffer "<<gluErrorString(glError)<<std::endl;
		}
		else
		{
			ilGenImages(1, &uImage);

			if (ilGetError() != IL_NO_ERROR)
			{
				std::cerr<<"Unable to create image"<<std::endl;
			}
			else
			{
				ilBindImage(uImage);

				if (ilGetError() != IL_NO_ERROR)
				{
					std::cerr<<"Unable to bind image"<<std::endl;
				}
				else
				{
					ilTexImage(g_iWidth,g_iHeight,1,4,IL_RGBA,IL_UNSIGNED_BYTE,(void*) &(vecImage.front()) );

					if (ilGetError() != IL_NO_ERROR)
					{
						std::cerr<<"Unable to store image"<<std::endl;
					}
					else
					{
						// Use DevIL to save a screenshot of the OpenGL window
						ilSaveImage(vcFilename);
	
						if (ilGetError() != IL_NO_ERROR)
						{
							std::cerr<<"Unable to save image"<<std::endl;
						}
						else
						{
							std::cout << "- Screenshot saved." << std::endl;
						}
					}
				}
			}
		}
		if (uImage)
		{
			ilBindImage(0);
			ilDeleteImages(1,&uImage);
		}
	}
}

// Exit function (some cleanup)
void glutClose()
{
	//todo: cleanup
}


const int GetNextPowerOfTwo(const int iNumber)
{
	int iPowerOfTwo = 1;

	while (iPowerOfTwo < iNumber)
		iPowerOfTwo = iPowerOfTwo * 2;

	return iPowerOfTwo;
};


// Entry point
int main(int argc, char **argv)
{
	std::cout << "Usage: VisLuFramework" << std::endl;
	std::string strDataFile("datafile");
	std::cout << "  [-"<<strDataFile<<"<filename>]" << std::endl<<std::endl;
	
	// Initialize GLUT and create window
	std::cout << "- Initializing GLUT ..." << std::endl;
	glutInit (&argc, argv);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize (g_iWidth,g_iHeight);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutCreateWindow("VisLuFramework");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, g_iWidth, g_iHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const unsigned char vpMessage[] = {"Initializing (see console for details) ..."};
	const int iMessageWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_12,vpMessage);
	const int iMessageHeight = glutBitmapHeight(GLUT_BITMAP_HELVETICA_12);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glRasterPos2f(-(float(iMessageWidth)/float(g_iWidth)),(float(iMessageHeight)/float(g_iHeight)));
	glutBitmapString(GLUT_BITMAP_HELVETICA_12,vpMessage);
	glRasterPos2f(0.0f,0.0f);	
	glutSwapBuffers();

	//set glut (callback-) functions
	glutIdleFunc (glutIdle);
	glutDisplayFunc (glutDisplay);
	glutReshapeFunc (glutReshape);
	glutMouseFunc (glutMouse);
	glutMotionFunc (glutMotion);
	glutKeyboardFunc (glutKeyboard);
	glutCloseFunc(glutClose);

	std::cout << "- GLUT initialized." << std::endl << std::endl;
	

	// Initialize GLEW
	std::cout << "- Initializing GLEW ..." << std::endl;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		// glewInit failed, something is seriously wrong
		std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "- GLEW initialized." << std::endl << std::endl;

	// Initialize DevIL (an image library)
	std::cout << "- Initializing DevIL ..." << std::endl;
	ilInit();
	if (ilGetError() != IL_NO_ERROR)
	{
		std::cerr << "Error initializing DevIL" << std::endl;
		return EXIT_FAILURE;
	}
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	std::cout << "- DevIL initialized." << std::endl << std::endl;

	//parse the console arguments
	g_argArguments = Arguments(argc,argv);
	
	if ( ! g_argArguments.HasOption(strDataFile)) //no datafile is specified
	{
		std::cout <<"No "<<strDataFile<<" specified: Nothing will be displayed!"<<std::endl;
	}
	else
	{
		std::string strFilename = g_argArguments.GetOption(strDataFile);
		//load volume
		g_Volume.load(strFilename);

		//create texture
		glGenTextures(1, &g_uExampleTexture);
		GLenum glError = GL_NO_ERROR;
		glError = glGetError();
		if (glError != GL_NO_ERROR)
		{
			std::cerr<<"Error generating texture: "<<gluErrorString(glError)<<std::endl;
		} 
		else if (g_uExampleTexture)
		{

			if(GLEW_EXT_texture3D)
			{
				g_fIncrement = 1.f / float(g_Volume.GetDepth()-1);
				//bind texture
				glBindTexture(GL_TEXTURE_3D, g_uExampleTexture);
				
				glError = glGetError();
				if (glError != GL_NO_ERROR)
				{
					std::cerr<<"Could not bind texture: "<<gluErrorString(glError)<<std::endl;
				}
				else
				{
					int iTextureWidth=g_Volume.GetWidth();
					int iTextureHeight=g_Volume.GetHeight();
					int iTextureDepth=g_Volume.GetDepth();

					if (!GLEW_ARB_texture_non_power_of_two)
					{
						iTextureWidth = GetNextPowerOfTwo(g_Volume.GetWidth());
						iTextureHeight = GetNextPowerOfTwo(g_Volume.GetHeight());
						iTextureDepth = GetNextPowerOfTwo(g_Volume.GetDepth());				 
					}

					//upload texture
					glTexImage3D(GL_TEXTURE_3D_EXT, 0, GL_LUMINANCE, iTextureWidth, iTextureHeight, iTextureDepth, 0, GL_LUMINANCE, GL_FLOAT, NULL);
					glTexSubImage3D(GL_TEXTURE_3D_EXT, 0, 0, 0, 0, g_Volume.GetWidth(), g_Volume.GetHeight(), g_Volume.GetDepth(), GL_LUMINANCE, GL_FLOAT, (void *) g_Volume.Get());
					
					glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glError = glGetError();
					if (glError != GL_NO_ERROR)
					{
						std::cerr<<"Could not upload texture: "<<gluErrorString(glError)<<std::endl;
					}
				}
			}
			else
			{
				std::cout<<"3D textures not supported!"<<std::endl;

				//bind texture
				glBindTexture(GL_TEXTURE_2D, g_uExampleTexture);

				glError = glGetError();
				if (glError != GL_NO_ERROR)
				{
					std::cerr<<"Could not bind texture: "<<gluErrorString(glError)<<std::endl;
				}
				else
				{
					int iTextureWidth=g_Volume.GetWidth();
					int iTextureHeight=g_Volume.GetHeight();
					int iTextureDepth=g_Volume.GetDepth();

					if (!GLEW_ARB_texture_non_power_of_two)
					{
						iTextureWidth = GetNextPowerOfTwo(g_Volume.GetWidth());
						iTextureHeight = GetNextPowerOfTwo(g_Volume.GetHeight());
						iTextureDepth = GetNextPowerOfTwo(g_Volume.GetDepth());				 
					}

					//upload 
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, iTextureWidth, iTextureHeight, 0, GL_LUMINANCE, GL_FLOAT, NULL);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_Volume.GetWidth(), g_Volume.GetHeight(), GL_LUMINANCE, GL_FLOAT, (void *) &(g_Volume.Get(0,0, int(float(g_Volume.GetDepth())*0.5f) )));
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glError = glGetError();
					if (glError != GL_NO_ERROR)
					{
						std::cerr<<"Could not upload texture: "<<gluErrorString(glError)<<std::endl;
					}
				}
			}
		}
		
		std::cout << std::endl;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();

	std::cout << "- Entering main loop ..." << std::endl<< std::endl;
	
	std::cout << "  Keyboard:" << std::endl;
	std::cout << "    [CTRL+G]             save screenshot" << std::endl<< std::endl;
	
	// Enter the main loop
	glutMainLoop();

	std::cout << "- Main loop terminated." << std::endl << std::endl;
	return EXIT_SUCCESS;
}

