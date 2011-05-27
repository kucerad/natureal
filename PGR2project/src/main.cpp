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
#include "settings.h"
#define TERRAIN_INIT_BORDER_VAL v4(13.0f, 10.0f, 5.0f, -1.0f)
#define TERRAIN_INIT_BORDER_WID v4(2.0f, 2.0f, 2.0f, 2.0f)

v4	g_terrain_border_values = TERRAIN_INIT_BORDER_VAL;
v4	g_terrain_border_widths = TERRAIN_INIT_BORDER_WID;

float tree2min = TREE2_MIN_HEIGHT;
float tree2max = TREE2_MAX_HEIGHT;
float tree1min = TREE1_MIN_HEIGHT;
float tree1max = TREE1_MAX_HEIGHT;
float grassmin = GRASS_MIN_HEIGHT;
float grassmax = GRASS_MAX_HEIGHT;

CameraMode g_cameraMode = TERRAIN_RESTRICTED;
int g_WinWidth				= 800;   // Window width
int g_WinHeight				= 600;   // Window height
double g_time				= 0.0;
CTimer						timer;
Statistics					g_Statistics;

#include "pgr2model.h"
#include <assert.h>
#include "../common/models/cube.h"
#include "World.h"

bool tqAvailable			= false;
GLuint tqid					= 0;
GLint result_available		= 0;

v3 g_light_position			= LIGHT_POSITION;
bool g_godraysEnabled		= false;
bool g_drawingReflection	= false;
bool g_showTextures			= false;

int	g_GrassCount			= GRASS_COUNT;
int	g_Tree1Count			= TREE1_COUNT;
int	g_Tree2Count			= TREE2_COUNT;

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


bool     g_ShowVertexNormals    = false; // Show vertex normal/tangent/binormal
bool     g_FaceNormals          = false; // Show face normal
bool     g_Transparency         = false; // Draw transparent meshes
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
   //if (g_FaceCulling) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);

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
   
   // if timer query available
   if (tqAvailable){
	    // measure on GPU
		//if (result_available){
			
			glBeginQuery(GL_TIME_ELAPSED, tqid);
		//}
		world.draw();		
		//if (result_available)
			glEndQuery(GL_TIME_ELAPSED);
		
		GLuint64EXT time = 0;
		glGetQueryObjectui64vEXT(tqid, GL_QUERY_RESULT, &time); // blocking CPU
		g_Statistics.fps = 1000000000.0/ double(time);
		//printf("FPS: %f\n",g_Statistics.fps);
		//glGetQueryObjectiv(tqid, GL_QUERY_RESULT_AVAILABLE, &result_available);
		//printf("avail: %s\n",result_available?"yes":"no");
   } else {
	    world.draw();
		// must block CPU to measure time here
		glFinish();
		double timeDiff = timer.RealTime() - g_time;
		g_Statistics.fps = 1.0 / (timeDiff);
   }
   

   
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

	// timer query extension?
	if (isExtensionSupported(TIME_QUERY_EXTENSION)){
		tqAvailable = true;
		glGenQueries(1, &tqid);
	}

}
void deinitApp()
{
	deletePlane();
	deleteCube();

	timer.Stop();

	//world.~World();
	if (tqAvailable){
		glDeleteQueries(1, &tqid);
	}
	printf("deinit done, bye\n");
	//system("PAUSE");
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
   glLineWidth(1.0f);

    

   // init app
   initApp();

   // Load model
   //loadNewModelCB(&g_ModelFileName);
}

void TW_CALL cbSetTree2Count(const void *value, void *clientData)
{ 
	g_Tree2Count = *(const int*)value; // for instance
	world.tree2_planter.plantVegetationCount(g_Tree2Count);
}
void TW_CALL cbGetTree2Count(void *value, void *clientData)
{ 
	*(int *)value = world.tree2_planter.count; // for instance
}

void TW_CALL cbSetTree1Count(const void *value, void *clientData)
{ 
	g_Tree1Count = *(const int*)value; // for instance
	world.tree1_planter.plantVegetationCount(g_Tree1Count);
}
void TW_CALL cbGetTree1Count(void *value, void *clientData)
{ 
	*(int *)value = world.tree1_planter.count; // for instance
}

void TW_CALL cbSetGrassCount(const void *value, void *clientData)
{ 
	g_GrassCount = *(const int*)value;  // for instance
	world.grass_planter.plantVegetationCount(g_GrassCount);
}
void TW_CALL cbGetGrassCount(void *value, void *clientData)
{ 
    *(int *)value = world.grass_planter.count;  // for instance
}

//tree2
void TW_CALL cbSetTree2Min(const void *value, void *clientData)
{ 
	tree2min = *(const float*)value;  // for instance
	world.tree2_planter.setNewMin(tree2min);
}
void TW_CALL cbGetTree2Min(void *value, void *clientData)
{ 
    *(float *)value = world.tree2_planter.height_min;  // for instance
}

void TW_CALL cbSetTree2Max(const void *value, void *clientData)
{ 
	tree2max = *(const float*)value;  // for instance
	world.tree2_planter.setNewMax(tree2max);
}
void TW_CALL cbGetTree2Max(void *value, void *clientData)
{ 
    *(float *)value = world.tree2_planter.height_max;  // for instance
}

// tree1
void TW_CALL cbSetTree1Min(const void *value, void *clientData)
{ 
	tree1min = *(const float*)value;  // for instance
	world.tree1_planter.setNewMin(tree1min);
}
void TW_CALL cbGetTree1Min(void *value, void *clientData)
{ 
    *(float *)value = world.tree1_planter.height_min;  // for instance
}

void TW_CALL cbSetTree1Max(const void *value, void *clientData)
{ 
	tree1max = *(const float*)value;  // for instance
	world.tree1_planter.setNewMax(tree1max);
}
void TW_CALL cbGetTree1Max(void *value, void *clientData)
{ 
    *(float *)value = world.tree1_planter.height_max;  // for instance
}


// grass
void TW_CALL cbSetGrassMin(const void *value, void *clientData)
{ 
	grassmin = *(const float*)value;  // for instance
	world.grass_planter.setNewMin(grassmin);
}
void TW_CALL cbGetGrassMin(void *value, void *clientData)
{ 
    *(float *)value = world.grass_planter.height_min;  // for instance
}

void TW_CALL cbSetGrassMax(const void *value, void *clientData)
{ 
	grassmax = *(const float*)value;  // for instance
	world.grass_planter.setNewMax(grassmax);
}
void TW_CALL cbGetGrassMax(void *value, void *clientData)
{ 
    *(float *)value = world.grass_planter.height_max;  // for instance
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
   TwDefine(" Controls position='10 10' size='250 370' refresh=0.1 \
            valueswidth=80 ");

   /*
   TwAddVarRW(controlBar, "model_file", TW_TYPE_STDSTRING, &g_ModelFileName,
      " label='file' group=Model help='Model file name.' ");
   TwAddButton(controlBar, "load_new_model", loadNewModelCB, NULL, 
      " label='load model' group=Model help='Load new model.' ");
   */
   TwAddVarRO(controlBar, "fps", TW_TYPE_FLOAT, &(g_Statistics.fps), 
	   " label='fps' group=Render help='frames per second' ");
   /*TwAddVarRW(controlBar, "x_translate", TW_TYPE_FLOAT, &(g_light_position.x), 
	   " label='x' group=Light help='x translation' ");
   TwAddVarRW(controlBar, "y_translate", TW_TYPE_FLOAT, &(g_light_position.y), 
	   " label='y' group=Light help='y translation' ");
   TwAddVarRW(controlBar, "z_translate", TW_TYPE_FLOAT, &(g_light_position.z), 
	   " label='z' group=Light help='z translation' ");   
	   */
   
   TwAddVarRW(controlBar, "fbos", TW_TYPE_BOOLCPP, &(g_showTextures), 
	   " label='Show FBOs' group=Debug help='enable/disable FBO display' "); 
   TwAddVarCB(controlBar, "Tree 2 count", TW_TYPE_INT16, cbSetTree2Count, cbGetTree2Count, NULL, " group='Vegetation' min=0 max=10000 step=1 ");
   TwAddVarCB(controlBar, "Tree 1 count", TW_TYPE_INT16, cbSetTree1Count, cbGetTree1Count, NULL, " group='Vegetation' min=0 max=10000 step=1 ");
   TwAddVarCB(controlBar, "Grass count", TW_TYPE_INT16, cbSetGrassCount, cbGetGrassCount, NULL, " group='Vegetation' min=0 max=10000 step=1 ");

    TwAddVarRW(controlBar, "Change1", TW_TYPE_FLOAT, &g_terrain_border_values.x, " group='Surfaces' label='Snow height' min=-5 max=30 step=0.5 help='Snow height' ");
	TwAddVarRW(controlBar, "Change1a", TW_TYPE_FLOAT, &g_terrain_border_widths.x, " group='Surfaces' label='Snow-rock transition' min=0 max=5 step=0.5 help='Transition 1' ");

    TwAddVarRW(controlBar, "Change2", TW_TYPE_FLOAT, &g_terrain_border_values.y, " group='Surfaces' label='Rock height' min=-5 max=30 step=0.5 help='Rock height' ");
	TwAddVarRW(controlBar, "Change2a", TW_TYPE_FLOAT, &g_terrain_border_widths.y, " group='Surfaces' label='Rock-grass transition' min=0 max=5 step=0.5 help='Transition 2' ");

    TwAddVarRW(controlBar, "Change3", TW_TYPE_FLOAT, &g_terrain_border_values.z, " group='Surfaces' label='Grass height' min=-5 max=30 step=0.5 help='Grass height' ");
	TwAddVarRW(controlBar, "Change3a", TW_TYPE_FLOAT, &g_terrain_border_widths.z, " group='Surfaces' label='Grass-clay transition' min=0 max=5 step=0.5 help='Transition 3' ");

    TwAddVarRW(controlBar, "Change4", TW_TYPE_FLOAT, &g_terrain_border_values.w, " group='Surfaces' label='Clay height' min=-5 max=30 step=0.5 help='Clay height' ");
	TwAddVarRW(controlBar, "Change4a", TW_TYPE_FLOAT, &g_terrain_border_widths.w, " group='Surfaces' label='Clay-ground transition' min=0 max=5 step=0.5 help='Transition 4' ");

   TwAddVarCB(controlBar, "Tree2 MIN", TW_TYPE_FLOAT, cbSetTree2Min, cbGetTree2Min, NULL, " group='Levels' min=-5 max=30 step=1 ");
   TwAddVarCB(controlBar, "Tree2 MAX", TW_TYPE_FLOAT, cbSetTree2Max, cbGetTree2Max, NULL, " group='Levels' min=-5 max=30 step=1 ");

   TwAddVarCB(controlBar, "Tree1 MIN", TW_TYPE_FLOAT, cbSetTree1Min, cbGetTree1Min, NULL, " group='Levels' min=-5 max=30 step=1 ");
   TwAddVarCB(controlBar, "Tree1 MAX", TW_TYPE_FLOAT, cbSetTree1Max, cbGetTree1Max, NULL, " group='Levels' min=-5 max=30 step=1 ");

   TwAddVarCB(controlBar, "Grass MIN", TW_TYPE_FLOAT, cbSetGrassMin, cbGetGrassMin, NULL, " group='Levels' min=-5 max=30 step=1 ");
   TwAddVarCB(controlBar, "Grass MAX", TW_TYPE_FLOAT, cbSetGrassMax, cbGetGrassMax, NULL, " group='Levels' min=-5 max=30 step=1 ");

   TwAddVarRW(controlBar, "godrays", TW_TYPE_BOOLCPP, &(g_godraysEnabled), 
	   " label='God rays enabled' group=Light help='enable/disable god rays' ");  

   //TwAddVarRW(controlBar, "vertex_normals", TW_TYPE_BOOLCPP, 
   //   &g_ShowVertexNormals, " label='vertex normals' \
   //   group=Render help='Show vertex normal, tangent, binormal.' ");
   //
   //TwAddVarRW(controlBar, "face_normals", TW_TYPE_BOOLCPP, &g_FaceNormals, 
   //   " label='face normals' group=Render help='Show face normals.' ");
   //TwAddVarRW(controlBar, "transparency", TW_TYPE_BOOLCPP, &g_Transparency, 
   //   " label='transparency' group=Render \
   //   help='Render transparent meshes.'");
   //TwAddVarRW(controlBar, "wiremode", TW_TYPE_BOOLCPP, &g_WireMode,
   //   " label='wire mode' group=Render help='Toggle wire mode.' ");
   //TwAddVarRW(controlBar, "face_culling", TW_TYPE_BOOLCPP, &g_FaceCulling,
   //   " label='face culling' group=Render  help='Toggle face culling.' ");
   //TwAddVarRW(controlBar, "alpha_threshold", TW_TYPE_FLOAT, &g_AlphaThreshold,
   //   " label='alpha threshold' group=Render min=0 max=1 step=0.01 \
   //    help='Alpha test threshold.' ");
   //TwAddVarRW(controlBar, "Translate", TW_TYPE_FLOAT, &g_SceneTraZ, 
   //   " group='Scene' label='translate Z' min=1 max=1000 step=0.5 \
   //    help='Scene translation.' ");
   //TwAddVarRW(controlBar, "Scale", TW_TYPE_FLOAT, &g_SceneScale, 
   //   " group='Scene' label='scale' min=0 max=10 step=0.01 \
   //    help='Scene scale.' ");
   //TwAddVarRW(controlBar, "SceneRotation", TW_TYPE_QUAT4F, &g_SceneRot, 
   //   " group='Scene' label='Scene rotation' open \
   //   help='Change the scene orientation.' ");
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
   world.windowSizeChanged(width,height);
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
	   /*
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
	  */
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
