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

#include "pgr2model.h"
#include <assert.h>
#include "World.h"

// GLOBAL CONSTANTS____________________________________________________________
const GLfloat VECTOR_RENDER_SCALE = 0.20f;
// GLOBAL VARIABLES____________________________________________________________
GLint    g_WinWidth             = 800;   // Window width
GLint    g_WinHeight            = 600;   // Window height

// Scene orientation (stored as a quaternion)
GLfloat  g_SceneRot[]           = {0.0f, 0.0f, 0.0f, 1.0f};   
GLfloat  g_SceneTraZ            = 10.0f; // Scene translation along z-axis
GLfloat  g_SceneScale           = 1.0f;

bool     g_ShowVertexNormals    =  true; // Show vertex normal/tangent/binormal
bool     g_FaceNormals          =  true; // Show face normal
bool     g_Transparency         =  true; // Draw transparent meshes
bool     g_WireMode             = false; // Wire mode enabled/disabled
bool     g_FaceCulling          = false; // Face culling enabled/disabled
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
   world.draw();
}
void initApp()
{
	world.init();

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
   glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
   glEnable(GL_DEPTH_TEST);
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_NORMALIZE);
   glLineWidth(2.0f);

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
      " label='load model' group=Model key=l help='Load new model.' ");

   TwAddVarRW(controlBar, "vertex_normals", TW_TYPE_BOOLCPP, 
      &g_ShowVertexNormals, " label='vertex normals' key=v \
      group=Render help='Show vertex normal, tangent, binormal.' ");
   TwAddVarRW(controlBar, "face_normals", TW_TYPE_BOOLCPP, &g_FaceNormals, 
      " label='face normals' group=Render key=f help='Show face normals.' ");
   TwAddVarRW(controlBar, "transparency", TW_TYPE_BOOLCPP, &g_Transparency, 
      " label='transparency' group=Render key=t \
      help='Render transparent meshes.'");
   TwAddVarRW(controlBar, "wiremode", TW_TYPE_BOOLCPP, &g_WireMode,
      " label='wire mode' group=Render key=t help='Toggle wire mode.' ");
   TwAddVarRW(controlBar, "face_culling", TW_TYPE_BOOLCPP, &g_FaceCulling,
      " label='face culling' group=Render key=c help='Toggle face culling.' ");
   TwAddVarRW(controlBar, "alpha_threshold", TW_TYPE_FLOAT, &g_AlphaThreshold,
      " label='alpha threshold' group=Render min=0 max=1 step=0.01 \
      keyIncr=a keyDecr=A help='Alpha test threshold.' ");
   TwAddVarRW(controlBar, "Translate", TW_TYPE_FLOAT, &g_SceneTraZ, 
      " group='Scene' label='translate Z' min=1 max=1000 step=0.5 \
      keyIncr=z keyDecr=Z help='Scene translation.' ");
   TwAddVarRW(controlBar, "Scale", TW_TYPE_FLOAT, &g_SceneScale, 
      " group='Scene' label='scale' min=0 max=10 step=0.01 \
      keyIncr=s keyDecr=S help='Scene scale.' ");
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
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(55.0f, GLfloat(width)/height, 1.0f, 100.0f);
   glMatrixMode(GL_MODELVIEW);

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
		  printf("MEZERA, ant=%s \n", g_MouseModeANT?"true":"false");
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
   static int s_LastMousePoxX = x;
   static int s_LastMousePoxY = y;

   if (g_MouseRotationEnabled)
   {
      g_SceneRot[1] +=  0.9f*(x - s_LastMousePoxX);
      g_SceneRot[2] +=  0.9f*(y - s_LastMousePoxY);
      s_LastMousePoxX = x;
      s_LastMousePoxY = y;
   }
   glfwSetMousePos(g_WinWidth/2, g_WinHeight/2);
}



//-----------------------------------------------------------------------------
// Name: main()
// Desc: 
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) 
{

   return common_main(g_WinWidth, g_WinHeight,
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
}