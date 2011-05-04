//-----------------------------------------------------------------------------
//  [PGR2] PGR2-Model Viewer
//  14/02/2011
//-----------------------------------------------------------------------------
//  Controls: 
//    [mouse-left]   ... scene rotation
//    [l]            ... (re)load model
//    [v]            ... toggle vertex TNB vectors
//    [f]            ... toggle face normal vectors
//    [t]            ... toggle transparency (draw transparent model parts)
//    [w]            ... toggle wire mode
//    [c]            ... toggle face culling
//    [a], [A]       ... increase/decrease alpha transparency threshold
//    [z], [Z]       ... increase/decrease scene translation along z axis
//    [s], [S]       ... increase/decrease scene scale
//-----------------------------------------------------------------------------
#define USE_ANTTWEAKBAR
#define TEST 0
#include "../common/Vector4.h"
#include "globals.h"
#define TERRAIN_INIT_BORDER_VAL v4(13.0f, 10.0f, 5.0f, -1.0f)
#define TERRAIN_INIT_BORDER_WID v4(0.6f, 1.5f, 0.4f, 2.f)

v4	g_terrain_border_values = TERRAIN_INIT_BORDER_VAL;
v4	g_terrain_border_widths = TERRAIN_INIT_BORDER_WID;

CameraMode g_cameraMode = WALK;
int g_WinWidth             = 800;   // Window width
int g_WinHeight            = 600;   // Window height
double g_time			   = 0.0;
CTimer timer;


#include "pgr2model.h"
#include <assert.h>
#include "../common/models/cube.h"
#include "World.h"

// GLOBAL CONSTANTS____________________________________________________________
const GLfloat VECTOR_RENDER_SCALE = 0.20f;
// GLOBAL VARIABLES____________________________________________________________


// Scene orientation (stored as a quaternion)
GLfloat  g_SceneRot[]           = {0.0f, 0.0f, 0.0f, 1.0f};   
GLfloat  g_SceneTraZ            = 10.0f; // Scene translation along z-axis
GLfloat  g_SceneScale           = 1.0f;

GLuint cube_vbo_id				= 0;
GLuint cube_ebo_id				= 0;
GLuint plane_vbo_id				= 0;
GLuint plane_ebo_id				= 0;


bool     g_ShowVertexNormals    =  false; // Show vertex normal/tangent/binormal
bool     g_FaceNormals          =  false; // Show face normal
bool     g_Transparency         =  false; // Draw transparent meshes
bool     g_WireMode             = false; // Wire mode enabled/disabled
bool     g_FaceCulling          = true; // Face culling enabled/disabled
GLfloat  g_AlphaThreshold       = 0.01f; // Alpha test threshold
bool	 g_MouseModeANT			= true;
// Model
PGR2Model* g_pModel             = NULL;
World world;

#include "../common/common.h"
// Model file name
std::string g_ModelFileName     = "models/DesertOasis/H1F.pgr2";


// FORWARD DECLARATIONS________________________________________________________
void initGUI(void);
void TW_CALL loadNewModelCB(void* clientData);
void TW_CALL copyStdStringToClient(std::string& dst, const std::string& src);

//-----------------------------------------------------------------------------
// Name: cbDisplay()
// Desc: 
//-----------------------------------------------------------------------------
void cbDisplay()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

   // Setup OpenGL states according to user settings
   glAlphaFunc(GL_GEQUAL, g_AlphaThreshold);

   glPolygonMode(GL_FRONT_AND_BACK, g_WireMode ? GL_LINE : GL_FILL);
   if (g_FaceCulling) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);

	// Setup camera
   glLoadIdentity();
   glTranslatef(0.5f, -1.0f, -g_SceneTraZ);
   pgr2AddQuaternionRotationToTransformation(g_SceneRot);
   glScalef(g_SceneScale, g_SceneScale, g_SceneScale);

   // Render model if exists
   if (g_pModel)
   {
      g_pModel->render(g_Transparency);

      if (g_ShowVertexNormals)   g_pModel->renderVertexNormals(VECTOR_RENDER_SCALE);
      if (g_FaceNormals)         g_pModel->renderFaceNormals(VECTOR_RENDER_SCALE);  
   }
   g_time=timer.RealTime();
   world.update(g_time);
   //printf("time: %fL\n", g_time);
   world.draw();
   
}

void initApp()
{
#if TEST
	// TEST START
	
	// do whatever u want... 

	system("PAUSE");
	
	exit(1);
	// TEST END
#endif
	timer.Reset();
	timer.Start();
	// set cube vbo
	initCube();
	//set plane vbo
	initPlane();

	world.init();

}
void deinitApp()
{
	deletePlane();
	deleteCube();

	timer.Stop();

	world.~World();
	printf("deinit done\n");
	system("PAUSE");
}

//-----------------------------------------------------------------------------
// Name: cbInitGL()
// Desc: 
//-----------------------------------------------------------------------------
void cbInitGL()
{
   // Init app GUI
   initGUI();

   // Set OpenGL state variables
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialfv(GL_FRONT_AND_BACK,  GL_AMBIENT, material_amd);
	glMaterialfv(GL_FRONT_AND_BACK,  GL_DIFFUSE, material_amd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spe);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
   glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glPointSize(1.f);
   glEnable(GL_LIGHTING);
   glLineWidth(1.0f);

    

   // init app
   initApp();

   // Load model
   //loadNewModelCB(&g_ModelFileName);
}


//-----------------------------------------------------------------------------
// Name: initGUI()
// Desc: 
//-----------------------------------------------------------------------------
void initGUI()
{
#ifdef USE_ANTTWEAKBAR
   // Initialize AntTweakBar GUI
   if (!TwInit(TW_OPENGL, NULL))
   {
      assert(0);
   }

   // Define the required callback function to copy a std::string 
   // (see TwCopyStdStringToClientFunc documentation)
   TwCopyStdStringToClientFunc(copyStdStringToClient); 
    
   TwWindowSize(g_WinWidth, g_WinHeight);
   TwBar *controlBar = TwNewBar("Controls");
   TwDefine(" Controls position='10 10' size='220 310' refresh=0.1 \
            valueswidth=80 ");

   TwAddVarRW(controlBar, "model_file", TW_TYPE_STDSTRING, &g_ModelFileName,
      " label='file' group=Model help='Model file name.' ");
   TwAddButton(controlBar, "load_new_model", loadNewModelCB, NULL, 
      " label='load model' group=Model help='Load new model.' ");

   TwAddVarRW(controlBar, "vertex_normals", TW_TYPE_BOOLCPP, 
      &g_ShowVertexNormals, " label='vertex normals' \
      group=Render help='Show vertex normal, tangent, binormal.' ");
   TwAddVarRW(controlBar, "face_normals", TW_TYPE_BOOLCPP, &g_FaceNormals, 
      " label='face normals' group=Render help='Show face normals.' ");
   TwAddVarRW(controlBar, "transparency", TW_TYPE_BOOLCPP, &g_Transparency, 
      " label='transparency' group=Render \
      help='Render transparent meshes.'");
   TwAddVarRW(controlBar, "wiremode", TW_TYPE_BOOLCPP, &g_WireMode,
      " label='wire mode' group=Render help='Toggle wire mode.' ");
   TwAddVarRW(controlBar, "face_culling", TW_TYPE_BOOLCPP, &g_FaceCulling,
      " label='face culling' group=Render  help='Toggle face culling.' ");
   TwAddVarRW(controlBar, "alpha_threshold", TW_TYPE_FLOAT, &g_AlphaThreshold,
      " label='alpha threshold' group=Render min=0 max=1 step=0.01 \
       help='Alpha test threshold.' ");
   TwAddVarRW(controlBar, "Translate", TW_TYPE_FLOAT, &g_SceneTraZ, 
      " group='Scene' label='translate Z' min=1 max=1000 step=0.5 \
       help='Scene translation.' ");
   TwAddVarRW(controlBar, "Scale", TW_TYPE_FLOAT, &g_SceneScale, 
      " group='Scene' label='scale' min=0 max=10 step=0.01 \
       help='Scene scale.' ");
   TwAddVarRW(controlBar, "SceneRotation", TW_TYPE_QUAT4F, &g_SceneRot, 
      " group='Scene' label='Scene rotation' open \
      help='Change the scene orientation.' ");
#endif
}


//-----------------------------------------------------------------------------
// Name: copyStdStringToClient()
// Desc: Function called to copy the content of a std::string (souceString) 
//       handled by the AntTweakBar library to destinationClientString handled 
//       by our application 
//-----------------------------------------------------------------------------
void TW_CALL copyStdStringToClient(std::string& dst, const std::string& src)
{
    dst = src;
} 


//-----------------------------------------------------------------------------
// Name: loadNewModelCB()
// Desc: Callback function to load new model
//-----------------------------------------------------------------------------
void TW_CALL loadNewModelCB(void* clientData)
{
   const std::string* file_name = &g_ModelFileName;//(const std::string *)(clientData);
   printf("RELOAD MODEL\n");
   if (!file_name->empty())
   {
      PGR2Model* pOldModel = g_pModel;
	  printf("LOAD: %s\n", (*file_name).c_str());
      g_pModel = PGR2Model::loadFromFile(file_name->c_str());
      if (g_pModel != NULL)
      {
         delete pOldModel;
      }
      else
      {
         g_pModel = pOldModel;
      }
   }
} 


//-----------------------------------------------------------------------------
// Name: cbWindowSizeChanged()
// Desc: 
//-----------------------------------------------------------------------------
void cbWindowSizeChanged(int width, int height)
{
   g_WinWidth  = width;
   g_WinHeight = height;
}
void activateANTMouse()
{
	glfwEnable( GLFW_MOUSE_CURSOR );
}
void activateGLFWMouse()
{
	//glfwDisable( GLFW_MOUSE_CURSOR );
	glfwSetMousePos(g_WinWidth/2, g_WinHeight/2);
}

//-----------------------------------------------------------------------------
// Name: cbKeyboardChanged()
// Desc: 
//-----------------------------------------------------------------------------
void cbKeyboardChanged(int key, int action)
{
	if (!g_MouseModeANT){
		// apply to camera first...
		if (world.p_activeCamera->handleKeyDown(key, action)){
			return;
		}
	}
   switch (key)
   {
      // DA use 'z' instead of 't'
      case 'z' : g_SceneTraZ  += 0.5f;                                   break;
      case 'Z' : g_SceneTraZ  -= (g_SceneTraZ > 0.5) ? 0.5f : 0.0f;      break;
      case 's' : g_SceneScale *= 1.01;                                   break;
      case 'S' : g_SceneScale *= 0.99;                                   break;
      case 'v' : g_ShowVertexNormals = !g_ShowVertexNormals;             break;
      case 'f' : g_FaceNormals != g_FaceNormals;                         break;
      case 't' : g_Transparency = !g_Transparency;                       break;
      case 'w' : g_WireMode    = !g_WireMode;                            break;
      case 'c' : g_FaceCulling = !g_FaceCulling;                         break;
      case 'a' : if(g_AlphaThreshold < 0.99f) g_AlphaThreshold += 0.01f; break;
      case 'A' : if(g_AlphaThreshold > 0.01f) g_AlphaThreshold -= 0.01f; break;
	  case ' ' : 
		  g_MouseModeANT = !g_MouseModeANT;
		  if (g_MouseModeANT){
			activateANTMouse();
		  } else {
			activateGLFWMouse();
		  }
		  break;
	}
}



bool g_MouseRotationEnabled = false;

//-----------------------------------------------------------------------------
// Name: cbMouseButtonChanged()
// Desc: internal
//-----------------------------------------------------------------------------
void GLFWCALL cbMouseButtonChanged(int button, int action)
{
   g_MouseRotationEnabled = ((button == GLFW_MOUSE_BUTTON_LEFT) && 
                             (action == GLFW_PRESS));
}


//-----------------------------------------------------------------------------
// Name: cbMousePositionChanged()
// Desc: 
//-----------------------------------------------------------------------------
void cbMousePositionChanged(int x, int y)
{
   
   world.p_activeCamera->handleMouseMove(x,y);
   glfwSetMousePos(g_WinWidth/2, g_WinHeight/2);
}



//-----------------------------------------------------------------------------
// Name: main()
// Desc: 
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) 
{
   int output = common_main(g_WinWidth, g_WinHeight,
                      "[PGR2] Semestral project",
                      cbInitGL,              // init GL callback function
                      cbDisplay,             // display callback function
                      cbWindowSizeChanged,   // window resize callback function
                      cbKeyboardChanged,     // keyboard callback function
#ifdef USE_ANTTWEAKBAR
                      cbMouseButtonChanged,                  // mouse button callback function
                      cbMousePositionChanged                  // mouse motion callback function
#else
                      cbMouseButtonChanged,  // mouse button callback function
                      cbMousePositionChanged // mouse motion callback function
#endif
                      );
   deinitApp();

   return output;
}
