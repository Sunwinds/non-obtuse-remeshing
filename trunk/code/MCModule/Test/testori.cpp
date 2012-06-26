#include "testori.h"

int last_visibility_state = GLUT_VISIBLE;
float xy_aspect = 0;
// mouse
const double gMouseStep = 0.2;
const double gMovementStep = 0.02;
int gMouseLastX = 0;
int gMouseLastY = 0;
bool gIsMoving = false;
double gRotX = 0;
double gRotY = 0;
double gTransX = 0;
double gTransY = 0;
double gTransZ = 0;
double gEyeX = 0;
double gEyeY = 0;
double gEyeZ = 1;
double gCenterX = 0;
double gCenterY = 0;
double gCenterZ = 0;
double gUpX = 0;
double gUpY = 1;
double gUpZ = 0;
int gWndWidth = 900;
int gWndHeight = 900;

/** These are the live variables passed into GLUI ***/
int   light0_enabled = 1;
int   gShadingType = 1;

char  open_text[] = "./mesh/test.smf";
char  save_text[] = "./mesh/test_mc.smf";
char  open_scalarField_text[] = "./mesh/test_sf.txt";
char  save_scalarField_text[] = "./mesh/test_sf.txt";
char  optimize_scalarField_text[] = "./mesh/test_sf_opt.txt";
char  optimize_debug_text[] = "./mesh/test_sf_deb.txt";

int   gShowObtuseAngles = 0;
int   degree = 0;
int   gShowDegree = 0;

int   gShowNonManifoldVertices = 0;
int   gShowNonManifoldEdges = 0;
int   gShowOpenManifoldEdges = 0;

float optn_alpha = 0.5;
int   optn_numIter = 0;
float optn_threshold = 0.0;
int   optn_numOneRingSearch = 3;
int   optn_numOneRingQuadric = 1;
int   optn_numMovePerIter = 1000;
int   gShowOriginalMesh = 1;
float optn_opacity = 0.25;
int   gOptnFirst = 1;
int   optn_numOptn = 0;
int   optn_numSmooth = 0;
int   gOptnRecompute = 0;
float anglePerBin = 10.0;
int   gOptnRecomputeClosestFace = 1;
int   optn_numSmoothingStep = 2;
float optn_decimate_threshold = 0.0;

float obj_scale = 1.0;

float light0_intensity = 1.0;

int   main_window;

int gShowCellsRegion = 0;
int gShowScalarField = 1;

float fCellLengthX = 0.01;
float fCellLengthY = 0.01;
float fCellLengthZ = 0.01;
int nCellX = 100;
int nCellY = 100;
int nCellZ = 100;
// armadillo
//float fCellLengthX = 1.5;
//float fCellLengthY = 1.5;
//float fCellLengthZ = 1.5;
//int nCellX = 90;
//int nCellY = 120;
//int nCellZ = 80;
// hand
//float fCellLengthX = 0.01;
//float fCellLengthY = 0.01;
//float fCellLengthZ = 0.01;
//int nCellX = 40;
//int nCellY = 40;
//int nCellZ = 120;
// horse
//float fCellLengthX = 0.0025;
//float fCellLengthY = 0.0025;
//float fCellLengthZ = 0.0025;
//int nCellX = 50;
//int nCellY = 80;
//int nCellZ = 70;
//float fCellLengthX = 0.005;
//float fCellLengthY = 0.005;
//float fCellLengthZ = 0.005;
//int nCellX = 25;
//int nCellY = 40;
//int nCellZ = 35;
//float fCellLengthX = 0.02;
//float fCellLengthY = 0.02;
//float fCellLengthZ = 0.02;
//int nCellX = 10;
//int nCellY = 10;
//int nCellZ = 10;

float fIsoValue = 0.0;

float obj_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
float obj_centre[] = { 0.0, 0.0, 0.0 };

float scene_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float scene_pos[] = { 0.0, 0.0, 0.0 };

float light_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

// mesh centroid
double* centroid = NULL;

int optn_decimate_numRemaining = 5000;
int optn_avoid_degree = 2;

float optn_improvement_threshold = 0.0;

float optn_angleBound = 30.0;
int gOptnAngleBound = 1;

int gCleanWhenStuck = 0;

//NORegionType g_region = ANGLEBOUND_7PLANES;

/** Pointers to the windows and some of the controls we'll create **/
/*
GLUI		*glui;

GLUI_Spinner	*obj_scale_spinner;
GLUI_Listbox	*obj_list;

GLUI_Rotation	 *view_rot_obj, *view_rot_objAll;
GLUI_Translation *trans_xy_obj, *trans_xy_objAll;
GLUI_Translation *trans_z_obj, *trans_z_objAll;

GLUI_Rotation	 *view_rot_light;

GLUI_Rotation	 *view_rot_scene;
GLUI_Translation *trans_xy_scene;
GLUI_Translation *trans_z_scene;

GLUI_Spinner     *light0_spinner;
GLUI_RadioGroup  *normal_radio_group, *shading_radio_group;

GLUI_RadioButton *norender_radio_button,
    *wireframe_radio_button,
    *flat_radio_button,
    *flat_wireframe_radio_button,
    *gouraud_radio_button,
    *gouraud_wireframe_radio_button;

GLUI_EditText *open_text_box;
GLUI_EditText *save_text_box;
GLUI_EditText *open_scalarField_box;
GLUI_EditText *save_scalarField_box;

GLUI_Button *open_button;
GLUI_Button *save_button;
GLUI_Button *open_sf_midpoint_button;
GLUI_Button *open_sf_interp_button;
GLUI_Button *open_sf_no_interp_button;
GLUI_Button *tiling_nonobtuse_button;
GLUI_Button *simple_MC_button;
GLUI_Button *save_scalarField_button;

GLUI_Checkbox *showObtuseAngles_checkbox;

GLUI_Button *update_obtuse_button;
GLUI_Button *blind_edgeflip_button;
GLUI_Button *greedy_edgeflip_button;
GLUI_Button *liftVertex_button;

GLUI_Spinner *vertex_degree_spinner;
GLUI_Checkbox *show_degree_checkbox;

GLUI_Button *moveVertexToCentroid_button;
GLUI_Button *moveVertexToMedian_button;

GLUI_Spinner *anglePerBin_spinner;
GLUI_Button *angle_histogram_button;

GLUI_Button *check_degeneracy_button;
GLUI_Button *check_geodegeneracy_button;
GLUI_Button *check_manifold_button;
GLUI_Button *check_closedmanifold_button;
GLUI_Checkbox *showNonManifoldVertices_checkbox;
GLUI_Checkbox *showNonManifoldEdges_checkbox;
GLUI_Checkbox *showOpenManifoldEdges_checkbox;

//GLUI_Button *marchingCube_button;

GLUI_Checkbox *showScalarField_checkbox;

GLUI_Checkbox *showCellsRegion_checkbox;
GLUI_Spinner *nCellsX_spinner;
GLUI_Spinner *nCellsY_spinner;
GLUI_Spinner *nCellsZ_spinner;
GLUI_Spinner *fCellLengthX_spinner;
GLUI_Spinner *fCellLengthY_spinner;
GLUI_Spinner *fCellLengthZ_spinner;
GLUI_Spinner *fIsoValue_spinner;

GLUI_Button *optimize_button;
GLUI_EditText *optimize_scalarField_box;
GLUI_Button *optimize_scalarField_button;

GLUI_Spinner *optn_numIter_spinner;
GLUI_Spinner *optn_alpha_spinner;
GLUI_Spinner *optn_threshold_spinner;
GLUI_Spinner *optn_numOneRingSearch_spinner;
GLUI_Spinner *optn_numOneRingQuadric_spinner;
GLUI_Spinner *optn_numMovePerIter_spinner;
GLUI_Checkbox *optn_showOriginal_checkbox;
GLUI_Spinner *optn_opacity_spinner;

GLUI_Button *removeBadValence_button;
GLUI_Button *optimize_smooth_button;
GLUI_Button *optimize_alternate_button;
GLUI_Checkbox *optimize_first_checkbox;
GLUI_Spinner *optn_numOptn_spinner;
GLUI_Spinner *optn_numSmooth_spinner;

GLUI_Button *optimize_smooth_new_button;
GLUI_Button *optimize_alternate_new_button;
GLUI_Button *optimize_new_button;

GLUI_Button* optimize_loadDebug_button;
GLUI_Button* optimize_saveDebug_button;

GLUI_Spinner* optn_decimate_remaining_spinner;
GLUI_Button* optimize_decimate_button;
GLUI_Spinner* optn_decimate_avoid_degree_spinner;
GLUI_Spinner* optn_decimate_threshold_spinner;

GLUI_Checkbox *optimize_recompute_checkbox;
GLUI_Checkbox *optimize_recomputeClosestFace_checkbox;
GLUI_Checkbox *optimize_angleBound_checkbox;
GLUI_Checkbox *optimize_cleanDirtyWhenStuck_checkbox;	

GLUI_Spinner* optn_improvement_threshold_spinner;
GLUI_Spinner* optn_angleBound_spinner;
GLUI_Spinner* optn_numSmoothStep_spinner;
*/

/********** User IDs for callbacks ********/
#define RESET_OBJ_ID			201
#define RESET_LIGHT_ID			204
#define RESET_SCENE_ID			205

#define NAV_ID				210
#define LIGHT_NAV_ID			211

#define OPEN_TEXTBOX_ID			220
#define OPEN_BUTTON_ID			221
#define SAVE_TEXTBOX_ID			230
#define SAVE_BUTTON_ID			231

#define OPEN_SCALARFIELD_TEXTBOX_ID	240
#define OPEN_SF_MIDPOINT_ID		241
#define OPEN_SF_INTERP_ID		242
#define OPEN_SF_NO_INTERP_ID		243
#define SAVE_SCALARFIELD_TEXTBOX_ID	244
#define SAVE_SCALARFIELD_ID		245
#define NCELLX_ID			246
#define NCELLY_ID			247
#define NCELLZ_ID			248
#define FCELLLENGTHX_ID			249
#define FCELLLENGTHY_ID			250
#define FCELLLENGTHZ_ID			251
#define FISOVALUE_ID			252
#define SHOW_CELLREGION_ID		253
#define SHOW_SCALARFIELD_ID		254
#define TILING_NONOBTUSE_ID		255
#define SIMPLE_MC_ID					256

#define SHADING_RADIOGROUP_ID		301

#define SHOW_OBTUSE_ID			400

#define UPDATE_OBTUSE_BUTTON_ID		401
#define BLIND_EDGEFLIP_BUTTON_ID 	402
#define GREEDY_EDGEFLIP_BUTTON_ID 	403
#define LIFT_VERTEX_BUTTON_ID 		404
#define DEGREE_ID 			405
#define SHOW_DEGREE_CHECKBOX_ID 	406
#define MOVE_VERTEX_CENTROID_BUTTON_ID 	407
#define MOVE_VERTEX_MEDIAN_BUTTON_ID 	408
//#define MARCHING_CUBE_BUTTON_ID 409
#define ANGLEPERBIN_ID			410
#define ANGLE_HISTOGRAM_BUTTON_ID	411
#define CHECK_DEGENERACY_ID			412
#define CHECK_GEODEGENERACY_ID		413
#define CHECK_MANIFOLD_ID			414
#define CHECK_CLOSEDMANIFOLD_ID		415
#define SHOW_NONMANIFOLDVERTICES_ID		416
#define SHOW_NONMANIFOLDEDGES_ID		417
#define SHOW_OPENMANIFOLDEDGES_ID		418

#define OPTN_ALPHA				500
#define OPTN_NUMITER_ID			501
#define OPTIMIZE_BUTTON_ID		502
#define OPTIMIZE_SCALARFIELD_TEXTBOX_ID	503
#define OPTIMIZE_SCALARFIELD_ID 504
#define OPTN_THRESHOLD			505
#define OPTN_NUMONERINGSEARCH_ID	506
#define OPTN_NUMONERINGQUADRIC_ID	507
#define OPTN_NUMMOVEPERITER_ID	508
#define SHOW_ORIGINALMESH_ID	509
#define OPTN_OPACITY_ID			510
#define OPTIMIZE_SMOOTH_BUTTON_ID	511
#define OPTIMIZE_ALTERNATE_BUTTON_ID	512
#define OPTN_FIRST_ID			513
#define OPTN_NUMOPTN_ID			514
#define OPTN_NUMSMOOTH_ID		515
#define OPTN_RECOMPUTE_ID		516
#define OPTN_RECOMPUTE_CLOSESTFACE_ID	517
#define OPTN_IMPROVEMENT_THRESHOLD	518
#define OPTN_ANGLEBOUND_ID		519
#define OPTIMIZE_ALTERNATE_NEW_BUTTON_ID	520
#define OPTIMIZE_SMOOTH_NEW_BUTTON_ID		521
#define OPTIMIZE_NEW_BUTTON_ID				522
#define REMOVE_BAD_VALENCES_BUTTON_ID		523
#define OPTN_CLEANWHENSTUCK_ID				524
#define OPTN_NUMSMOOTHINGSTEP_ID			525
#define OPTN_DECIMATE_THRESHOLD				526

#define OPTIMIZE_LOADDEBUG_BUTTON_ID	600
#define OPTIMIZE_SAVEDEBUG_BUTTON_ID	601
#define OPTIMIZE_DEBUG_TEXTBOX_ID		602

#define OPTN_DECIMATE_NUMREMAINING_ID	700
#define OPTIMIZE_DECIMATE_BUTTON_ID		701
#define OPTN_DECIMATE_AVOID_DEGREE_ID	702


/********** Miscellaneous global variables **********/

//GLfloat light0_ambient[] =  {1.0f, 1.0f, 1.0f, 1.0f};
//GLfloat light0_diffuse[] =  {.7f, .7f, .7f, .7f};
//GLfloat light0_specular[] = {.95f, .95f, .8f, .9f};
//GLfloat light0_position[] = {-7.0f, -1.0f, 10.0f, 0.0f};

GLfloat light0_ambient[] =  {.5f, .5f, .5f, .5f};
GLfloat light0_diffuse[] =  {.7f, 1.0f, 1.0f, .7f};
GLfloat light0_specular[] = {.5f, .5f, .5f, .5f};
GLfloat light0_position[] = {-7.0f, -1.0f, 10.0f, 0.0f};

// Renderer class instance
Renderer g_rdr;
// Nonobtuse class instance
Nonobtuse g_nonObtuse;
// CIsoSurface class instance
CNonobtuseMC<float> g_isoSurface;
// NonobtOptn class instance
//NonobtOptn g_optn;
// edges that make up an obtuse angle
vector<Edge> obtuseEdges;
// vertices where an obtuse angle occurs
vector<int> obtuseVertices;
// vertices highlighted by degrees
vector<int> verticesByDegree;
// non-manifold vertices
vector<int> nonManifoldVertices;
// non-manifold edges
vector<Edge> nonManifoldEdges;
// open-manifold edges
vector<Edge> openManifoldEdges;
// for rendering the original mesh with transparency
Renderer g_original_rdr;
// TilingNonobtuseMC class
TilingNonobtuseMC<float> g_tilingNOMC;

// function declaration
void resetProperties();
void updateObtuseAngles();
void control_cb( int control );
void myGlutVisibility(int state);
void myGlutKeyboard(unsigned char Key, int x, int y);
void myGlutMenu( int value );
void myGlutIdle( void );
void myGlutMouse(int button, int button_state, int x, int y );
void myGlutMotion(int x, int y );
void myGlutReshape( int x, int y );
void myGlutDisplay( void );
void glui_init();
void glut_init();
bool init(int argc, char* argv[]);


void resetProperties()
{
    /*
    obj_scale_spinner->set_float_val(1.0);
    shading_radio_group->set_int_val(1);
    showObtuseAngles_checkbox->set_int_val(0);
    vertex_degree_spinner->set_int_val(0);
    show_degree_checkbox->set_int_val(0);

    // reset object rotation
    view_rot_obj->reset();

    // reset object translation
    trans_xy_obj->set_x(0.0);
    trans_xy_obj->set_y(0.0);
    trans_xy_obj->set_z(0.0);
    trans_z_obj->set_x(0.0);
    trans_z_obj->set_y(0.0);
    trans_z_obj->set_z(0.0);

    // reset light rotation
    view_rot_light->reset();

    // reset scene rotation
    view_rot_scene->reset();

    // reset scene translation
    trans_xy_scene->set_x(0.0);
    trans_xy_scene->set_y(0.0);
    trans_xy_scene->set_z(0.0);
    trans_z_scene->set_x(0.0);
    trans_z_scene->set_y(0.0);
    trans_z_scene->set_z(0.0);8
    */
}

void updateObtuseAngles()
{
    // find obtuse angles
    g_nonObtuse.clearObtuseAngles();
    g_nonObtuse.findObtuseAngles(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());

    obtuseEdges.clear();
    obtuseVertices.clear();

    const vector<ObtuseAngle> obAngle = g_nonObtuse.getObtuseAngles();
    cout << "\nTotal number of obtuse angles: " << obAngle.size() << endl;

    double avgVertexDegrees(0.0);
    double avgObtuseAngleDegrees(0.0);
    double maxObtuseAngle(0.0);
    double minObtuseAngle(-1.0);
    for (vector<ObtuseAngle>::const_iterator i = obAngle.begin(); i != obAngle.end(); ++i)
    {
        avgVertexDegrees += g_rdr.getVertexDegree(i->vertexIdx);
        double angle = getAngle(g_rdr.getVertices()[3*(i->vertexIdx)], 
            g_rdr.getVertices()[3*(i->vertexIdx)+1],
            g_rdr.getVertices()[3*(i->vertexIdx)+2],
            g_rdr.getVertices()[3*(i->neighbour1Idx)],
            g_rdr.getVertices()[3*(i->neighbour1Idx)+1],
            g_rdr.getVertices()[3*(i->neighbour1Idx)+2],
            g_rdr.getVertices()[3*(i->neighbour2Idx)],
            g_rdr.getVertices()[3*(i->neighbour2Idx)+1],
            g_rdr.getVertices()[3*(i->neighbour2Idx)+2]);
        avgObtuseAngleDegrees += angle;

        if (angle > maxObtuseAngle)
            maxObtuseAngle = angle;
        if (angle < minObtuseAngle || minObtuseAngle < 0)
            minObtuseAngle = angle;

        Edge e;
        e.vertex1Idx = i->vertexIdx;
        e.vertex2Idx = i->neighbour1Idx;
        obtuseEdges.push_back(e);					
        e.vertex2Idx = i->neighbour2Idx;
        obtuseEdges.push_back(e);

        obtuseVertices.push_back(i->vertexIdx);
    }

    if (obAngle.size() > 0)
    {
        avgVertexDegrees /= obAngle.size();
        avgObtuseAngleDegrees /= obAngle.size();

        printf("Average vertex degrees: %lf\n", avgVertexDegrees);
        printf("Average obtuse angle degrees: %lf degrees (%lf radians)\n", avgObtuseAngleDegrees*180.0/M_PI, avgObtuseAngleDegrees);
        printf("Max obtuse angle: %lf degrees (%lf radians)\n", maxObtuseAngle*180.0/M_PI, maxObtuseAngle);
        printf("Min obtuse angle: %lf degrees (%lf radians)\n", minObtuseAngle*180.0/M_PI, minObtuseAngle);

        // 		cout << "Obtuse vertices: " << endl;
        // 		for (unsigned int i = 0; i < obtuseVertices.size(); ++i)
        // 			cout << obtuseVertices[i] << endl;

        // 		cout << "Average vertex degrees: " << avgVertexDegrees << endl;
        // 		cout << "Average obtuse angle degrees: " << avgObtuseAngleDegrees*180.0/M_PI << " degrees (" << avgObtuseAngleDegrees << " radians)\n";
        // 		cout << "Max obtuse angle: " << maxObtuseAngle*180.0/M_PI << " degrees (" << maxObtuseAngle << " radians)\n";
        // 		cout << "Min obtuse angle: " << minObtuseAngle*180.0/M_PI << " degrees (" << minObtuseAngle << " radians)\n";
    }
}


/*************************************************************************/
/*              GLUI control callback                                    */
/*************************************************************************/

void control_cb( int control )
{
    if (control == NAV_ID )		// navigation control
    {
    }
    else if (control == LIGHT_NAV_ID )	// light navigation control
    {
    }
    else if ( control == RESET_OBJ_ID )	// reset object control
    {
        //view_rot_obj->reset();
        //trans_xy_obj->set_x(0.0);
        //trans_xy_obj->set_y(0.0);
        //trans_xy_obj->set_z(0.0);
        //trans_z_obj->set_x(0.0);
        //trans_z_obj->set_y(0.0);
        //trans_z_obj->set_z(0.0);
    }
    else if ( control == RESET_LIGHT_ID )	// reset light control
    {
        /*view_rot_light->reset();*/
    }
    else if ( control == RESET_SCENE_ID )	// reset scene control
    {
        //view_rot_scene->reset();
        //trans_xy_scene->set_x(0.0);
        //trans_xy_scene->set_y(0.0);
        //trans_xy_scene->set_z(0.0);
        //trans_z_scene->set_x(0.0);
        //trans_z_scene->set_y(0.0);
        //trans_z_scene->set_z(0.0);
    }
    else if (control == SHADING_RADIOGROUP_ID)	// shading mode control
    {
    }
    else if (control == DEGREE_ID)			// degree spinner
    {
        if (gShowDegree != 0 && g_rdr.isLoaded())
        {
            verticesByDegree.clear();
            g_rdr.getVerticesByDegree(degree, verticesByDegree);
            cout << "Number of vertices with degree-" << degree << ": " << verticesByDegree.size() << endl;
        }
    }
    else if (control == SHOW_DEGREE_CHECKBOX_ID)	// show degree checkbox
    {
        if (gShowDegree != 0 && g_rdr.isLoaded())
        {
            verticesByDegree.clear();
            g_rdr.getVerticesByDegree(degree, verticesByDegree);
            cout << "Number of vertices with degree-" << degree << ": " << verticesByDegree.size() << endl;
        }
    }
    else if (control == SHOW_OBTUSE_ID)		// show obtuse angles
    {
    }
    else if (control == UPDATE_OBTUSE_BUTTON_ID)	// update obtuse angles
    {
        updateObtuseAngles();
    }
    else if (control == BLIND_EDGEFLIP_BUTTON_ID)	// blind edge flip
    {
        g_nonObtuse.blindEdgeFlips(g_rdr);
        updateObtuseAngles();
    }
    else if (control == GREEDY_EDGEFLIP_BUTTON_ID)  // greedy edge flip
    {
        g_nonObtuse.greedyEdgeFlips(g_rdr);
        updateObtuseAngles();
    }
    else if (control == LIFT_VERTEX_BUTTON_ID)	// lift vertex
    {
        g_nonObtuse.liftVertices(g_rdr);
        updateObtuseAngles();
    }
    else if (control == MOVE_VERTEX_CENTROID_BUTTON_ID) 	// move vertex to centroid
    {
        g_nonObtuse.moveVerticesToCentroid(g_rdr);
        updateObtuseAngles();
    }
    else if (control == MOVE_VERTEX_MEDIAN_BUTTON_ID) 	// move vertex to median
    {
        g_nonObtuse.moveVerticesToMedian(g_rdr);
        updateObtuseAngles();
    }
    else if (control == ANGLE_HISTOGRAM_BUTTON_ID)		// angle histogram
    {
        vector<int> angleHistogram;
        g_nonObtuse.getAngleHistogram(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), anglePerBin, angleHistogram);

        float angle = 0.0;
        cout << "Angle Histogram: " << endl;
        for (unsigned int i = 0; i < angleHistogram.size(); ++i)
        {
            cout << "[" << angle << " - ";
            angle += anglePerBin;
            if (angle > 180.0)
                angle = 180.0;
            cout << angle << "): ";
            cout << angleHistogram[i] << endl;
        }
    }
    else if (control == CHECK_DEGENERACY_ID)
    {
        checkDegenerateCases(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), false);
    }
    else if (control == CHECK_GEODEGENERACY_ID)
    {
        checkDegenerateCases(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), true);
    }
    else if (control == CHECK_MANIFOLD_ID)
    {
        checkManifold(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), false, nonManifoldVertices, nonManifoldEdges, openManifoldEdges);
    }
    else if (control == CHECK_CLOSEDMANIFOLD_ID)
    {
        checkManifold(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), true, nonManifoldVertices, nonManifoldEdges, openManifoldEdges);
    }
    /*
    else if (control == OPTN_ANGLEBOUND_ID)
    {	
        if (gOptnAngleBound)
            g_region = ANGLEBOUND_7PLANES;
        else
            g_region = SIMPLE3PLANES_TOWARD_CENTER;
    }
    else if (control == REMOVE_BAD_VALENCES_BUTTON_ID)
    {
        if (g_optn.isLoaded())
            g_optn.removeBadValence(g_region, optn_angleBound);
        else
            cout << "No mesh is loaded in the mesh optimizer" << endl;
    }
    else if (control == OPTIMIZE_ALTERNATE_BUTTON_ID || control == OPTIMIZE_ALTERNATE_NEW_BUTTON_ID)	// optimize alternate (optn + smooth)
    {
        if (!g_optn.isLoaded())
        {
            if (control == OPTIMIZE_ALTERNATE_NEW_BUTTON_ID)
            {
                g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);
                // 				g_tilingNOMC.GenerateSurface_simpleMC(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);

                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_tilingNOMC.getVertices(novList);
                int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

                g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons, g_tilingNOMC.getClosestTri());

                // remove bad valences
                g_optn.removeBadValence(g_region, optn_angleBound);

                // free up g_tilingNOMC
                g_tilingNOMC.DeleteSurface();

                // original mesh
                g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());
            }
            else if (control == OPTIMIZE_ALTERNATE_BUTTON_ID && strcmp(optimize_scalarField_text, "") != 0 && 
                g_isoSurface.GenerateSurface(optimize_scalarField_text, fIsoValue, MIDPOINT))
            {
                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_isoSurface.getVertices(novList);
                int numNoPolygons = g_isoSurface.getTriangles(nopList);

                g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

                // remove bad valences
                g_optn.removeBadValence(g_region, optn_angleBound);

                // free up g_isoSurface
                g_isoSurface.DeleteSurface();

                // original mesh
                g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());
            }
            else
            {
                cout << "Failed loading scalar field file: " << optimize_scalarField_text << endl;
            }
        }

        // optimize in alternate fashion
        for (int i = 0; i < optn_numSmoothingStep; ++i)
        {
            g_optn.optimize_smoothing(optn_numIter, optn_threshold, optn_angleBound, gOptnRecompute, g_region);

            // 			if (i == optn_numSmoothingStep-1)
            // 				break;

            // 			g_optn.optimize_afterMove(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, true, gOptnRecomputeClosestFace, false, g_region);
            g_optn.optimize_afterMove_noPriority(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, true, gOptnRecomputeClosestFace, false, g_region);

            // 			g_optn.optimize_smoothing(optn_numIter, optn_threshold, optn_angleBound, gOptnRecompute, g_region);
        }

        // 		g_optn.optimize_afterMove(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, true, gOptnRecomputeClosestFace, true, g_region);
        g_optn.optimize_afterMove_noPriority(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, true, gOptnRecomputeClosestFace, false, g_region);

        // copy vertices back to renderer
        g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());

    }
    else if (control == OPTIMIZE_SMOOTH_BUTTON_ID || control == OPTIMIZE_SMOOTH_NEW_BUTTON_ID)	// optimize smoothing
    {
        if (g_optn.isLoaded())
        {
            // optimize
            g_optn.optimize_smoothing(optn_numIter, optn_threshold, optn_angleBound, gOptnRecompute, g_region);

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else if (control == OPTIMIZE_SMOOTH_NEW_BUTTON_ID)
        {
            g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);

            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_tilingNOMC.getVertices(novList);
            int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

            g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons, g_tilingNOMC.getClosestTri());
            //g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

            // free up g_tilingNOMC
            g_tilingNOMC.DeleteSurface();

            // remove bad valences
            g_optn.removeBadValence(g_region, optn_angleBound);

            // optimize
            g_optn.optimize_smoothing(optn_numIter, optn_threshold, optn_angleBound, gOptnRecompute, g_region);

            // original mesh
            g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else if (control == OPTIMIZE_SMOOTH_BUTTON_ID && strcmp(optimize_scalarField_text, "") != 0 && 
            g_isoSurface.GenerateSurface(optimize_scalarField_text, fIsoValue, MIDPOINT))
        {
            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_isoSurface.getVertices(novList);
            int numNoPolygons = g_isoSurface.getTriangles(nopList);

            g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

            // free up g_isoSurface
            g_isoSurface.DeleteSurface();

            // remove bad valences
            g_optn.removeBadValence(g_region, optn_angleBound);

            // optimize
            g_optn.optimize_smoothing(optn_numIter, optn_threshold, optn_angleBound, gOptnRecompute, g_region);

            // original mesh
            g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else
        {
            cout << "Failed loading scalar field file: " << optimize_scalarField_text << endl;
        }
    }
    else if (control == OPTIMIZE_BUTTON_ID || control == OPTIMIZE_NEW_BUTTON_ID)			// optimize button
    {
        if (g_optn.isLoaded())
        {
            // optimize
            g_optn.optimize_afterMove(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, gOptnRecompute, gOptnRecomputeClosestFace, gCleanWhenStuck, g_region);
            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else if (control == OPTIMIZE_NEW_BUTTON_ID)
        {	
            g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);

            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_tilingNOMC.getVertices(novList);
            int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

            g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons, g_tilingNOMC.getClosestTri());
            //g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

            // free up g_tilingNOMC
            g_tilingNOMC.DeleteSurface();

            // remove bad valences
            g_optn.removeBadValence(g_region, optn_angleBound);

            // optimize
            g_optn.optimize_afterMove(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, gOptnRecompute, gOptnRecomputeClosestFace, gCleanWhenStuck, g_region);

            // original mesh
            g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else if (control == OPTIMIZE_BUTTON_ID && strcmp(optimize_scalarField_text, "") != 0 && 
            g_isoSurface.GenerateSurface(optimize_scalarField_text, fIsoValue, MIDPOINT))
        {
            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_isoSurface.getVertices(novList);
            int numNoPolygons = g_isoSurface.getTriangles(nopList);

            g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

            // free up g_isoSurface
            g_isoSurface.DeleteSurface();

            // remove bad valences
            g_optn.removeBadValence(g_region, optn_angleBound);

            // optimize
            g_optn.optimize_afterMove(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, gOptnRecompute, gOptnRecomputeClosestFace, gCleanWhenStuck, g_region);

            // original mesh
            g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else
        {
            cout << "Failed loading scalar field file: " << optimize_scalarField_text << endl;
        }
    }
    else if (control == OPTIMIZE_SCALARFIELD_ID)	// optimize scalar field button
    {
        if (strcmp(optimize_scalarField_text, "") != 0)
        {
            if (g_isoSurface.GenerateSurface(optimize_scalarField_text, fIsoValue, MIDPOINT))
            {
                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_isoSurface.getVertices(novList);
                int numNoPolygons = g_isoSurface.getTriangles(nopList);

                g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

                // free up g_isoSurface
                g_isoSurface.DeleteSurface();

                // remove bad valences
                g_optn.removeBadValence(g_region, optn_angleBound);

                // optimize
                g_optn.optimize_afterMove(optn_numIter, optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, gOptnRecompute, gOptnRecomputeClosestFace, gCleanWhenStuck, g_region);

                // copy vertices back to renderer
                g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());

                // original mesh
                g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());

                // free up g_optn
                g_optn.reset();
            }
            else
            {
                cout << "Failed loading scalar field file: " << optimize_scalarField_text << endl;
            }
        }
    }
    */
    else if (control == SHOW_CELLREGION_ID)			// show cells region
    {
    }
    else if (control == SHOW_SCALARFIELD_ID)		// show scalar field
    {
    }
    // 	else if (control == MARCHING_CUBE_BUTTON_ID)        // marching cube  ***** OBSOLETE *****
    // 	{
    // 		float *scalarField = NULL;
    // 		vector<lineMeshIntType>* xIntPtList = NULL;
    // 		vector<lineMeshIntType>* yIntPtList = NULL;
    // 		vector<lineMeshIntType>* zIntPtList = NULL;
    // 		double xoffsets, yoffsets, zoffsets;
    // 		g_nonObtuse.getScalarField(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), scalarField, xIntPtList, yIntPtList, zIntPtList, xoffsets, yoffsets, zoffsets, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);
    // 		
    // 		g_isoSurface.GenerateSurface(scalarField, xIntPtList, yIntPtList, zIntPtList, xoffsets, yoffsets, zoffsets, fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);
    // 		
    // 		double* verticesList = NULL;
    // 		int** trianglesList = NULL;
    // 		int numVertices = g_isoSurface.getVertices(verticesList);
    // 		int numTriangles = g_isoSurface.getTriangles(trianglesList);
    // 		
    // 		resetProperties(); 
    // 		g_rdr.loadMeshInfo(verticesList, numVertices, trianglesList, numTriangles);
    // 		updateObtuseAngles();
    // 
    // 		delete[] scalarField;
    // 		delete[] xIntPtList;
    // 		delete[] yIntPtList;
    // 		delete[] zIntPtList;
    // 		
    // 		// TODO: should do some clean up
    // 		//g_isoSurface.DeleteSurface();
    // 	}
    else if (control == OPEN_SF_MIDPOINT_ID ||	// open scalarfield button (midpoint)
        control == OPEN_SF_INTERP_ID ||		// open scalarfield button (interpolation)
        control == OPEN_SF_NO_INTERP_ID)	// open scalarfield button (nonobtuse interpolation)
    {
        if (strcmp(open_scalarField_text, "") != 0)
        {
            bool result;
            if (control == OPEN_SF_MIDPOINT_ID)
                result = g_isoSurface.GenerateSurface(open_scalarField_text, fIsoValue, MIDPOINT);
            else if (control == OPEN_SF_INTERP_ID)
                result = g_isoSurface.GenerateSurface(open_scalarField_text, fIsoValue, INTERPOLATE);
            else
                result = g_isoSurface.GenerateSurface(open_scalarField_text, fIsoValue, NONOBTUSE_INTERPOLATE);

            if (result)
            {
                double* verticesList = NULL;
                int** trianglesList = NULL;
                int numVertices = g_isoSurface.getVertices(verticesList);
                int numTriangles = g_isoSurface.getTriangles(trianglesList);

                resetProperties(); 
                g_rdr.loadMeshInfo(verticesList, numVertices, trianglesList, numTriangles);
                updateObtuseAngles();
            }
            else
            {
                cout << "Failed loading scalar field file: " << open_scalarField_text << endl;
            }
        }

        // TODO: should do some clean up
        //g_isoSurface.DeleteSurface();
    }
    else if (control == TILING_NONOBTUSE_ID)		// tiling nonobtuse
    {
        if (g_rdr.isLoaded())
        {
            g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);

            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_tilingNOMC.getVertices(novList);
            int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

            // ---- debug only ----
            g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());
            // ---- end debug -----

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(novList, numNoVertices, nopList, numNoPolygons);			
        }
    }
    else if (control == SIMPLE_MC_ID)			// simple nonobtuse MC
    {
        if (g_rdr.isLoaded())
        {
            g_tilingNOMC.GenerateSurface_simpleMC(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), fIsoValue, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ);

            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_tilingNOMC.getVertices(novList);
            int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

            // ---- debug only ----
            g_original_rdr.loadMeshInfo(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons());
            // ---- end debug -----

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(novList, numNoVertices, nopList, numNoPolygons);			
        }
    }
    else if (control == SAVE_SCALARFIELD_ID)	// save scalarfield button
    {
        if (g_rdr.isLoaded())
        {	
            // check if mesh is open
            vector<int> holesOneRingList;
            unsigned int numHoles = g_nonObtuse.getHoles(g_rdr.getVertices(), g_rdr.getNumVertices(), 
                g_rdr.getPolygons(), g_rdr.getNumPolygons(), 
                g_rdr.getEdgeNeighbourList(), holesOneRingList);

            if (numHoles > 0)	// open meshes
            {
                double* newVList = NULL;
                int numNewVertices = 0;
                int ** newPList = NULL;
                int numNewPolygons = 0;

                // fill up hole
                if (g_nonObtuse.fillHoles(g_rdr.getVertices(), g_rdr.getNumVertices(), 
                    g_rdr.getPolygons(), g_rdr.getNumPolygons(), 
                    holesOneRingList, numHoles,
                    newVList, numNewVertices,
                    newPList, numNewPolygons))
                {
                    // write scalar field
                    if (!g_nonObtuse.writeScalarField(newVList, numNewVertices, newPList, numNewPolygons, save_scalarField_text, nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ, g_rdr.getNumPolygons()))
                        cout << "Failed writing scalar field file: " << save_scalarField_text << endl;

                    g_rdr.loadMeshInfo(newVList, numNewVertices, newPList, numNewPolygons);
                }
                else
                {
                    cout << "Failed filling holes in open mesh." << endl;
                }

                // clean up
                if (newVList != NULL)
                    delete[] newVList;
                if (newPList != NULL)
                {
                    for (int i = 0; i < numNewPolygons; ++i)
                        delete[] newPList[i];
                    delete[] newPList;
                }
            }
            else				// closed meshes
            {
                if (!g_nonObtuse.writeScalarField(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), save_scalarField_text, nCellX, 	nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ))
                    cout << "Failed writing scalar field file: " << save_scalarField_text << endl;
            }

        }
    }
    else if (control == OPEN_BUTTON_ID)	// open button
    {
        if (strcmp(open_text, "") != 0)
        {
            resetProperties();  // reset feature threshold before load file
            if (!g_rdr.loadFile(open_text))
            {
                // can't load file
                cout << "Failed loading file: " << open_text << endl;
            }
            else
            {
                // file loaded
                updateObtuseAngles();
                //g_optn.reset();
            }
        }
    }
    else if (control == SAVE_BUTTON_ID)
    {
        if (strcmp(save_text, "") != 0)
            g_rdr.saveFile(save_text);
    }
    /*
    else if (control == OPTIMIZE_LOADDEBUG_BUTTON_ID)
    {
        g_optn.loadDebug(optimize_debug_text);
        g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
    }
    else if (control == OPTIMIZE_SAVEDEBUG_BUTTON_ID)
    {
        g_optn.saveDebug(optimize_debug_text);
    }
    else if (control == OPTIMIZE_DECIMATE_BUTTON_ID)
    {
        if (g_optn.isLoaded())
        {
            // decimate
            g_optn.decimate(optn_decimate_numRemaining, optn_decimate_threshold, optn_angleBound, optn_avoid_degree, optn_alpha, g_region);
            // 			g_optn.decimate_MC_LinearSearch(optn_decimate_numRemaining, optn_decimate_threshold, optn_angleBound, optn_avoid_degree, optn_alpha);
            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), 
                g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
        }
        else
        {
            cout << "No mesh is loaded in optimizer" << endl;
        }
    }
    */

    glutPostRedisplay();
}

/**************************************** myGlutVisibility() ********/

void myGlutVisibility(int state)
{
    if (last_visibility_state == GLUT_NOT_VISIBLE && state == GLUT_VISIBLE)
        glutPostRedisplay();

    last_visibility_state = state;
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
    case 27:
    case 'q':
        exit(0);
        break;
    case 'a':
        {
            // open file
            SMFParser smfparser;
            if(!smfparser.loadFile(open_text))
            {
                cout << "file is not found!" << endl;
                return;
            }
            vector<vector<double>> vList;
            vector<vector<int>> fList;
            if(!smfparser.parse(vList, fList))
            {
                cout << "data is incorrect!" << endl;
                return;
            }
            // normalize
            double xMin = 32767, xMax = 0;
            double yMin = 32767, yMax = 0;
            double zMin = 32767, zMax = 0;
            double x = 0;
            double y = 0;
            double z = 0;
            for(vector<vector<double>>::iterator vIter = vList.begin(); vIter != vList.end(); ++vIter)
            {
                vector<double>::iterator iter = vIter->begin();
                x = *iter;
                if(x < xMin) xMin = x;
                if(x > xMax) xMax = x;
                ++iter;
                double y = *iter;
                if(y < yMin) yMin = y;
                if(y > yMax) yMax = y;
                ++iter;
                double z = *iter;
                if(z < zMin) zMin = z;
                if(z > zMax) zMax = z;
            }
            double xLen = xMax - xMin;
            double yLen = yMax - yMin;
            double ratio = 1.0 / max(xLen, yLen);
            double centerX = (xMax - xMin) / 2 * ratio;
            double centerY = (yMax - yMin) / 2 * ratio;
            double centerZ = (zMax - zMin) / 2 * ratio;
            for(vector<vector<double>>::iterator vIter = vList.begin(); vIter != vList.end(); ++vIter)
            {
                vector<double>::iterator iter = vIter->begin();
                *iter = (*iter - xMin) * ratio - centerX;
                ++iter;
                *iter = (*iter - yMin) * ratio - centerX;
                ++iter;
                *iter = (*iter - zMin) * ratio - centerX;
            }
            // save file
            ofstream outFile;
            outFile.open(open_text, ofstream::out);
            outFile.setf(ofstream::fixed, ofstream::floatfield);
            outFile.precision(6);
            // first line records the number of vertices and number of polygons
            outFile << "#$SMF 1.0" << endl;
            outFile << "#$vertices " << vList.size() << endl;
            outFile << "#$faces " << fList.size() << endl;
            // record list of vertices
            for(vector<vector<double>>::iterator vIter = vList.begin(); vIter != vList.end(); ++vIter)
            {
                outFile << 'v';
                for(vector<double>::iterator iter = vIter->begin(); iter != vIter->end(); ++iter)
                {
                    outFile << ' ' << *iter;
                }
                outFile << endl;
            }
            // record list of polygons
            for(vector<vector<int>>::iterator fIter = fList.begin(); fIter != fList.end(); ++fIter)
            {
                outFile << 'f';
                for(vector<int>::iterator iter = fIter->begin(); iter != fIter->end(); ++iter)
                {
                    outFile << ' ' << *iter;
                }
                outFile << endl;
            }
            outFile.close();
        }
        cout << "done!" << endl;
        break;
    case 'b':
        {
            const double* vList = g_rdr.getVertices();
            int vCount = g_rdr.getNumVertices();
            double xMin = 32767, xMax = 0;
            double yMin = 32767, yMax = 0;
            double zMin = 32767, zMax = 0;
            const double* vPtr = vList;
            for(int i = 0; i < vCount; ++i)
            {
                double x = *vPtr;
                if(x < xMin) xMin = x;
                if(x > xMax) xMax = x;
                ++vPtr;
                double y = *vPtr;
                if(y < yMin) yMin = y;
                if(y > yMax) yMax = y;
                ++vPtr;
                double z = *vPtr;
                if(z < zMin) zMin = z;
                if(z > zMax) zMax = z;
                ++vPtr;
            }
            double xLen = xMax - xMin;
            double yLen = yMax - yMin;
            double zLen = zMax - zMin;
            double minLen = min(xLen, min(yLen, zLen));
            double step = minLen / 20;
            fCellLengthX = step;
            fCellLengthY = step;
            fCellLengthZ = step;
            nCellX = int(xLen / step + 0.5) + 4;
            nCellY = int(yLen / step + 0.5) + 4;
            nCellZ = int(zLen / step + 0.5) + 4;
            cout << "nCellX: " << nCellX << endl;
            cout << "nCellY: " << nCellY << endl;
            cout << "nCellZ: " << nCellZ << endl;
            cout << "fCellLengthX: " << fCellLengthX << endl;
            cout << "fCellLengthY: " << fCellLengthY << endl;
            cout << "fCellLengthZ: " << fCellLengthZ << endl;
        }
        cout << "done!" << endl;
        break;
    case 'c': // create scalar field
        control_cb(SAVE_SCALARFIELD_ID);
        cout << "done!" << endl;
        break;
    case 'o': // open marching cube
        control_cb(OPEN_SF_MIDPOINT_ID);
        cout << "done!" << endl;
        break;
    case 's': // save marching cube
        control_cb(SAVE_BUTTON_ID);
        cout << "done!" << endl;
        break;
    case 'u': // move up
        gEyeY += gMovementStep;
        break;
    case 'j': // move down
        gEyeY -= gMovementStep;
        break;
    case 'h': // move left
        gEyeX -= gMovementStep;
        break;
    case 'k': // move right
        gEyeX += gMovementStep;
        break;
    case 'y': // move far
        gEyeZ += gMovementStep;
        break;
    case 'i': // move near
        gEyeZ -= gMovementStep;
        break;
    default:
        break;
    };

    glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
    myGlutKeyboard( value, 0, 0 );
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
    if ( glutGetWindow() != main_window )
        glutSetWindow(main_window);

    glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y )
{
    //int specialKey = glutGetModifiers();
    switch(button_state)
    {
    case GLUT_DOWN:
        {
            switch(button)
            {
            case GLUT_LEFT_BUTTON: // for the left button
                {
                    gMouseLastX = x;
                    gMouseLastY = y;
                    gIsMoving = true;
                }
                break;
            case GLUT_MIDDLE_BUTTON: // for the middle button
                break;
            case GLUT_RIGHT_BUTTON: // for the right button
                break;
            default:
                break;
            }
        }
        break;
    case GLUT_UP:
        {
            switch(button)
            {
            case GLUT_LEFT_BUTTON: // for the left button
                {
                    gIsMoving = false;
                }
                break;
            case GLUT_MIDDLE_BUTTON: // for the middle button
                break;
            case GLUT_RIGHT_BUTTON: // for the right button
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y )
{
    if(gIsMoving)
    {
        double newPosX = x;
        double newPosY = y;
        double oldPosX = gMouseLastX;
        double oldPosY = gMouseLastY;
        double deltaX = (newPosX - oldPosX) * gMouseStep;
        double deltaY = (newPosY - oldPosY) * gMouseStep;
        double angle = gRotX + deltaY;
        gRotX = angle > 360.0f ? angle - 360.0f : angle;
        angle = gRotY + deltaX;
        gRotY = angle > 360.0f ? angle - 360.0f : angle;
        gMouseLastX = x;
        gMouseLastY = y;
        glutPostRedisplay();
    }
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
    //glViewport( 0, 0, x, y );
    gWndWidth = x;
    gWndHeight = y;
    //xy_aspect = (float)x / (float)y;
    glutPostRedisplay();
}


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
    //glClearColor( .9f, .9f, .8f, 1.0f );
    //glClearColor( .95f, .95f, .9f, 1.0f );
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // default viewport
    glViewport( 0, 0, gWndWidth, gWndHeight );

    xy_aspect = (float)gWndWidth / (float)gWndHeight;

    // projection  
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum( -xy_aspect * 0.01, xy_aspect * 0.01, -0.01, 0.01, 0.1, 500.0 );
    gluPerspective(30.0f, xy_aspect, 1.0f, 100.0f);


    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(
        gEyeX, gEyeY, gEyeZ,
        gCenterX, gCenterY, gCenterZ,
        gUpX, gUpY, gUpZ);

    // scene transformation
    //glTranslatef( 0.0, 0.0, -5.0f );
    glTranslatef( 0.0, 0.0, -1.0f );
    glTranslatef( scene_pos[0], scene_pos[1], -scene_pos[2] );
    glRotated(gRotX, 1.0f, 0.0f, 0.0f);
    glRotated(gRotY, 0.0f, 1.0f, 0.0f);
    glMultMatrixf( scene_rotate );

    // light transformation
    glPushMatrix();
    glMultMatrixf( light_rotate );
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glPopMatrix();

    // viewport
    glViewport( 0, 0, gWndWidth, gWndHeight );

    // render object here ---------------------------------

    // start with the object in the center
    bool gotCentroid(true);
    gotCentroid = g_rdr.getCentroid(centroid);
    if (gotCentroid)
        glTranslatef(-centroid[0], -centroid[1], -centroid[2]);

    glPushMatrix();

    // object translation
    glTranslatef(obj_pos[0], obj_pos[1], -obj_pos[2]);

    // scale and rotate about mesh centroid if available
    if (gotCentroid)
    {
        glTranslatef(centroid[0], centroid[1], centroid[2]);
        glScalef(obj_scale, obj_scale, obj_scale);
        glMultMatrixf(obj_rotate);
        glTranslatef(-centroid[0], -centroid[1], -centroid[2]);
    }
    else
    {
        glScalef(obj_scale, obj_scale, obj_scale);
        glMultMatrixf(obj_rotate);
    }

    // shading mode
    int shadeMode(0);
    /*
    if (shading_radio_group->get_int_val() == 1)        // wireframe
    {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        g_rdr.render(shadeMode);
        glEnable(GL_LIGHTING);
    }
    else if (shading_radio_group->get_int_val() == 2)   // flat shading
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        shadeMode = 1;

        g_rdr.render(shadeMode);
    }
    else if (shading_radio_group->get_int_val() == 3)   // flat shading with wireframe
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPolygonOffset(-2.5f, -2.5f);

        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_LINE);
        g_rdr.render();
        glEnable(GL_LIGHTING);

        glPopAttrib();

        glPolygonMode(GL_FRONT, GL_FILL);
        shadeMode = 1;
        g_rdr.render(shadeMode);
    }
    else if (shading_radio_group->get_int_val() == 4)   // gouraud shading
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        shadeMode = 0;

        g_rdr.render(shadeMode);
    }                                                   // gouraud shading with wireframe
    else if (shading_radio_group->get_int_val() == 5)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPolygonOffset(-0.5f, -0.5f);

        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_LINE);
        g_rdr.render();
        glEnable(GL_LIGHTING);

        glPopAttrib();

        glPolygonMode(GL_FRONT, GL_FILL);
        shadeMode = 0;
        g_rdr.render(shadeMode);
    }
    */
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPolygonOffset(-0.5f, -0.5f);

        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_LINE);
        g_rdr.render();
        glEnable(GL_LIGHTING);

        glPopAttrib();

        glPolygonMode(GL_FRONT, GL_FILL);
        shadeMode = 0;
        g_rdr.render(shadeMode);

    // render scalar field
    if (gShowScalarField && g_isoSurface.IsSurfaceValid())
    {
        g_rdr.renderScalarField(g_isoSurface.getScalarField(), g_isoSurface.getNumCellX(), g_isoSurface.getNumCellY(), g_isoSurface.getNumCellZ(), g_isoSurface.getCellLengthX(), g_isoSurface.getCellLengthY(), g_isoSurface.getCellLengthZ(), g_isoSurface.getXOffset(), g_isoSurface.getYOffset(), g_isoSurface.getZOffset(), g_isoSurface.getIsoLevel());
    }

    // render cells region
    if (gShowCellsRegion)
    {
        float mcBoxLength[3] = {(nCellX*fCellLengthX),(nCellY*fCellLengthY),(nCellZ*fCellLengthZ)};
        double xoffset = centroid[0] - (mcBoxLength[0] / 2.0);
        double yoffset = centroid[1] - (mcBoxLength[1] / 2.0);
        double zoffset = centroid[2] - (mcBoxLength[2] / 2.0);
        g_rdr.renderCellInfo(nCellX, nCellY, nCellZ, fCellLengthX, fCellLengthY, fCellLengthZ, xoffset, yoffset, zoffset, false);
    }

    // render obtuse angles
    if (gShowObtuseAngles)
    {
        g_rdr.renderEdges(obtuseEdges, 0.25);
        g_rdr.renderPts(obtuseVertices);
    }

    // render vertices by degree
    if (gShowDegree)
    {
        g_rdr.renderPts(verticesByDegree);
    }

    // render non-manifold vertices
    if (gShowNonManifoldVertices)
    {
        g_rdr.renderPts(nonManifoldVertices);
    }

    // render non-manifold edges
    if (gShowNonManifoldEdges)
    {
        g_rdr.renderEdges(nonManifoldEdges, 1.0);
    }

    // render open-manifold edges
    if (gShowOpenManifoldEdges)
    {
        g_rdr.renderEdges(openManifoldEdges, 1.0);
    }

    // render original mesh
    if (gShowOriginalMesh)
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        glDisable(GL_LIGHTING);
        glColor4f(0.0, 0.7, 0.7, optn_opacity);
        g_original_rdr.render(1);	// flat shading
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

////////////////////////////////////////////////////////////////////////////
/****************************************/
/*         Setup GLUI code              */
/****************************************/

void glui_init()
{
    ///*** Create the side subwindow ***/
    //glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );

    //GLUI_Rollout *panel_prop = glui->add_rollout( "Properties" , true);

    ///***** Control for object params *****/
    //glui->add_statictext_to_panel(panel_prop, "Model properties");

    //obj_scale_spinner = glui->add_spinner_to_panel( panel_prop, "Scale:    ", GLUI_SPINNER_FLOAT, &obj_scale, NAV_ID, control_cb);
    //obj_scale_spinner->set_float_limits( 0.0, 50.0 );
    //obj_scale_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //obj_scale_spinner->set_speed( .03 );

    ///***** Nonobtuse Angles *****/
    //glui->add_separator_to_panel(panel_prop);
    //GLUI_Panel *nonobtuse_angles_rollout = glui->add_rollout_to_panel(panel_prop, "Nonobtuse Angles", false);

    //vertex_degree_spinner = glui->add_spinner_to_panel( nonobtuse_angles_rollout, "Degree:    ", GLUI_SPINNER_INT, &degree, DEGREE_ID, control_cb);
    //vertex_degree_spinner->set_int_limits( 0, 100 );
    //vertex_degree_spinner->set_speed( .5 );
    //vertex_degree_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //show_degree_checkbox = glui->add_checkbox_to_panel(nonobtuse_angles_rollout, "Show Vertices by Valence", &gShowDegree, SHOW_DEGREE_CHECKBOX_ID, control_cb);

    //showObtuseAngles_checkbox = glui->add_checkbox_to_panel(nonobtuse_angles_rollout, "Show Obtuse Angles", &gShowObtuseAngles, SHOW_OBTUSE_ID, control_cb );
    //update_obtuse_button = glui->add_button_to_panel(nonobtuse_angles_rollout, "Update Obtuse Angles", UPDATE_OBTUSE_BUTTON_ID, control_cb);

    //anglePerBin_spinner = glui->add_spinner_to_panel( nonobtuse_angles_rollout, "Angles Per Bin:", GLUI_SPINNER_FLOAT, &anglePerBin, ANGLEPERBIN_ID, control_cb);
    //anglePerBin_spinner->set_float_limits( 0.01, 180 );
    //angle_histogram_button = glui->add_button_to_panel(nonobtuse_angles_rollout, "Angle Histogram", ANGLE_HISTOGRAM_BUTTON_ID, control_cb);

    //GLUI_Panel *checkMesh_rollout = glui->add_rollout_to_panel(panel_prop, "Check Mesh", false);
    //GLUI_Panel *checkMeshButton_panel = glui->add_panel_to_panel(checkMesh_rollout, "");
    //check_degeneracy_button = glui->add_button_to_panel(checkMeshButton_panel, "Degeneracy", CHECK_DEGENERACY_ID, control_cb);
    //check_geodegeneracy_button = glui->add_button_to_panel(checkMeshButton_panel, "Geo-Degeneracy", CHECK_GEODEGENERACY_ID, control_cb);
    //glui->add_column_to_panel(checkMeshButton_panel, false);
    //check_manifold_button = glui->add_button_to_panel(checkMeshButton_panel, "Manifold", CHECK_MANIFOLD_ID, control_cb);
    //check_closedmanifold_button = glui->add_button_to_panel(checkMeshButton_panel, "Closed-Manifold", CHECK_CLOSEDMANIFOLD_ID, control_cb);
    //showNonManifoldVertices_checkbox = glui->add_checkbox_to_panel(checkMesh_rollout, "Show Non-Manifold Vertices", &gShowNonManifoldVertices, SHOW_NONMANIFOLDVERTICES_ID, control_cb );
    //showNonManifoldEdges_checkbox = glui->add_checkbox_to_panel(checkMesh_rollout, "Show Non-Manifold Edges", &gShowNonManifoldEdges, SHOW_NONMANIFOLDEDGES_ID, control_cb );
    //showOpenManifoldEdges_checkbox = glui->add_checkbox_to_panel(checkMesh_rollout, "Show Open-Manifold Edges", &gShowOpenManifoldEdges, SHOW_OPENMANIFOLDEDGES_ID, control_cb );

    //GLUI_Rollout *misc_rollout = glui->add_rollout_to_panel(panel_prop, "Misc." , false);

    //blind_edgeflip_button = glui->add_button_to_panel(misc_rollout, "Blind Edge Flip", BLIND_EDGEFLIP_BUTTON_ID, control_cb);
    //greedy_edgeflip_button = glui->add_button_to_panel(misc_rollout, "Greedy Edge Flip", GREEDY_EDGEFLIP_BUTTON_ID, control_cb);
    //liftVertex_button = glui->add_button_to_panel(misc_rollout, "Lift Vertex", LIFT_VERTEX_BUTTON_ID, control_cb);
    //moveVertexToCentroid_button = glui->add_button_to_panel(misc_rollout, "Move Vertex to Centroid", MOVE_VERTEX_CENTROID_BUTTON_ID, control_cb);
    //moveVertexToMedian_button = glui->add_button_to_panel(misc_rollout, "Move Vertex to Median", MOVE_VERTEX_MEDIAN_BUTTON_ID, control_cb);


    ///***** Render controls *****/
    //glui->add_column_to_panel( panel_prop );
    //glui->add_statictext_to_panel( panel_prop , "Render control");

    //shading_radio_group = glui->add_radiogroup_to_panel(panel_prop, &gShadingType, SHADING_RADIOGROUP_ID, control_cb);

    //norender_radio_button = glui->add_radiobutton_to_group(shading_radio_group, "Hide Mesh");
    //wireframe_radio_button = glui->add_radiobutton_to_group(shading_radio_group, "Wireframe");
    //flat_radio_button = glui->add_radiobutton_to_group(shading_radio_group, "Flat Shading");
    //flat_wireframe_radio_button = glui->add_radiobutton_to_group(shading_radio_group, "Flat Shading + wireframe");
    //gouraud_radio_button = glui->add_radiobutton_to_group(shading_radio_group, "Gouraud Shading");
    //gouraud_wireframe_radio_button = glui->add_radiobutton_to_group(shading_radio_group, "Gouraud Shading + wireframe");

    ///***** Optimization Controls *****/
    //GLUI_Rollout *opt_rollout = glui->add_rollout( "Optimization" , false);
    //GLUI_Panel *opt_panel = glui->add_panel_to_panel(opt_rollout, "");
    //optn_threshold_spinner = glui->add_spinner_to_panel( opt_panel, "Threshold:    ", GLUI_SPINNER_FLOAT, &optn_threshold, OPTN_THRESHOLD, control_cb);
    //optn_threshold_spinner->set_float_limits( 0.0, 100.0 );
    //optn_threshold_spinner->set_speed( .03 );
    //optn_improvement_threshold_spinner = glui->add_spinner_to_panel( opt_panel, "Improv. Th'd: ", GLUI_SPINNER_FLOAT, &optn_improvement_threshold, OPTN_IMPROVEMENT_THRESHOLD, control_cb);
    //optn_improvement_threshold_spinner->set_float_limits( 0.0, 100.0 );
    //optn_improvement_threshold_spinner->set_speed( .03 );
    //optn_alpha_spinner = glui->add_spinner_to_panel( opt_panel, "Alpha:    ", GLUI_SPINNER_FLOAT, &optn_alpha, OPTN_ALPHA, control_cb);
    //optn_alpha_spinner->set_float_limits( 0.0, 1.0 );
    //optn_alpha_spinner->set_speed( .03 );
    //optn_numIter_spinner = glui->add_spinner_to_panel( opt_panel, "Iterations: ", GLUI_SPINNER_INT, &optn_numIter, OPTN_NUMITER_ID, control_cb);
    //optn_numIter_spinner->set_int_limits( 0, 100000 );
    //optn_numIter_spinner->set_speed( 0.1 );
    //glui->add_column_to_panel(opt_panel, false);
    //optn_numOneRingSearch_spinner = glui->add_spinner_to_panel( opt_panel, "Num 1-Ring Search: ", GLUI_SPINNER_INT, &optn_numOneRingSearch, OPTN_NUMONERINGSEARCH_ID, control_cb);
    //optn_numOneRingSearch_spinner->set_int_limits( 1, 100);
    //optn_numOneRingSearch_spinner->set_speed( 0.01 );
    //optn_numOneRingQuadric_spinner = glui->add_spinner_to_panel( opt_panel, "Num 1-Ring Quadric: ", GLUI_SPINNER_INT, &optn_numOneRingQuadric, OPTN_NUMONERINGQUADRIC_ID, control_cb);
    //optn_numOneRingQuadric_spinner->set_int_limits( 0, 100);
    //optn_numOneRingQuadric_spinner->set_speed( 0.01 );
    //optn_numMovePerIter_spinner = glui->add_spinner_to_panel( opt_panel, "NumMove/Iter: ", GLUI_SPINNER_INT, &optn_numMovePerIter, OPTN_NUMMOVEPERITER_ID, control_cb);
    //optn_numMovePerIter_spinner->set_int_limits( 1, 100000);
    //optn_numMovePerIter_spinner->set_speed( 1 );
    //optn_angleBound_spinner = glui->add_spinner_to_panel( opt_panel, "Angle Bound: ", GLUI_SPINNER_FLOAT, &optn_angleBound, OPTN_ANGLEBOUND_ID, control_cb);
    //optn_angleBound_spinner->set_float_limits( 0.0, 90.0);
    //optn_angleBound_spinner->set_speed( 1 );

    //GLUI_Panel *opt_panel2 = glui->add_panel_to_panel( opt_rollout, "");
    //optimize_recompute_checkbox = glui->add_checkbox_to_panel(opt_panel2, "Recompute Quadrics/Smoothness", &gOptnRecompute, OPTN_RECOMPUTE_ID, control_cb );
    //optimize_recomputeClosestFace_checkbox = glui->add_checkbox_to_panel(opt_panel2, "Recompute closest face", &gOptnRecomputeClosestFace, OPTN_RECOMPUTE_CLOSESTFACE_ID, control_cb );
    //optimize_first_checkbox = glui->add_checkbox_to_panel(opt_panel2, "Optimize First", &gOptnFirst, OPTN_FIRST_ID, control_cb );
    //optn_numOptn_spinner = glui->add_spinner_to_panel( opt_panel2, "Num Opt'n: ", GLUI_SPINNER_INT, &optn_numOptn, OPTN_NUMOPTN_ID, control_cb);
    //optn_numOptn_spinner->set_int_limits( 0, 100000);
    //optn_numOptn_spinner->set_speed( 0.01 );
    //optn_numSmooth_spinner = glui->add_spinner_to_panel( opt_panel2, "Num Smooth: ", GLUI_SPINNER_INT, &optn_numSmooth, OPTN_NUMSMOOTH_ID, control_cb);
    //optn_numSmooth_spinner->set_int_limits( 0, 100000);
    //optn_numSmooth_spinner->set_speed( 0.01 );
    //// **** WARNING: the remove-bad-valence feature will destroy the data for optimization and decimation; so after using it will need to recompute data for optimization and decimation ****
    ////removeBadValence_button = glui->add_button_to_panel(opt_panel2, "Remove bad valences", REMOVE_BAD_VALENCES_BUTTON_ID, control_cb);


    //GLUI_Panel *opt_panel3 = glui->add_panel_to_panel(opt_panel2, "Optimization");
    //optimize_smooth_button = glui->add_button_to_panel(opt_panel3, "Smoothing", OPTIMIZE_SMOOTH_BUTTON_ID, control_cb);
    //optimize_button = glui->add_button_to_panel(opt_panel3, "Optimize", OPTIMIZE_BUTTON_ID, control_cb);
    //optimize_alternate_button = glui->add_button_to_panel(opt_panel3, "Alternate", OPTIMIZE_ALTERNATE_BUTTON_ID, control_cb);
    //optimize_angleBound_checkbox = glui->add_checkbox_to_panel(opt_panel3, "with angle bound", &gOptnAngleBound, OPTN_ANGLEBOUND_ID, control_cb );
    //optimize_cleanDirtyWhenStuck_checkbox = glui->add_checkbox_to_panel(opt_panel3, "clean vertices when stuck", &gCleanWhenStuck, OPTN_CLEANWHENSTUCK_ID, control_cb );	
    //optn_numSmoothStep_spinner = glui->add_spinner_to_panel( opt_panel3, "Num Smoothing Step: ", GLUI_SPINNER_INT, &optn_numSmoothingStep, OPTN_NUMSMOOTHINGSTEP_ID, control_cb);
    //optn_numSmoothStep_spinner->set_int_limits( 0, 100);
    //optn_numSmoothStep_spinner->set_speed( 1 );
    //glui->add_column_to_panel(opt_panel3, false);
    //optimize_smooth_new_button = glui->add_button_to_panel(opt_panel3, "Smoothing (New)", OPTIMIZE_SMOOTH_NEW_BUTTON_ID, control_cb);
    //optimize_new_button = glui->add_button_to_panel(opt_panel3, "Optimize (New)", OPTIMIZE_NEW_BUTTON_ID, control_cb);
    //optimize_alternate_new_button = glui->add_button_to_panel(opt_panel3, "Alternate (New)", OPTIMIZE_ALTERNATE_NEW_BUTTON_ID, control_cb);

    //GLUI_Panel *opt_panel4 = glui->add_panel_to_panel( opt_rollout, "");
    //optn_decimate_remaining_spinner = glui->add_spinner_to_panel( opt_panel4, "Num Vertices: ", GLUI_SPINNER_INT, &optn_decimate_numRemaining, OPTN_DECIMATE_NUMREMAINING_ID, control_cb);
    //optn_decimate_remaining_spinner->set_int_limits( 1, 100000);
    //optn_decimate_remaining_spinner->set_speed( 0.01 );

    //optn_decimate_avoid_degree_spinner = glui->add_spinner_to_panel( opt_panel4, "Avoid degrees: ", GLUI_SPINNER_INT, &optn_avoid_degree, OPTN_DECIMATE_AVOID_DEGREE_ID, control_cb);
    //optn_decimate_avoid_degree_spinner->set_int_limits( 2, 100);
    //optn_decimate_avoid_degree_spinner->set_speed( 0.01 );
    //optn_decimate_threshold_spinner = glui->add_spinner_to_panel( opt_panel4, "Threshold:    ", GLUI_SPINNER_FLOAT, &optn_decimate_threshold, OPTN_DECIMATE_THRESHOLD, control_cb);
    //optn_decimate_threshold_spinner->set_float_limits( 0.0, 100.0 );
    //optn_decimate_threshold_spinner->set_speed( .03 );
    //optimize_decimate_button = glui->add_button_to_panel(opt_panel4, "Decimate Mesh", OPTIMIZE_DECIMATE_BUTTON_ID, control_cb);

    //optimize_scalarField_box = glui->add_edittext_to_panel(opt_rollout, "Scalar Field:", GLUI_EDITTEXT_TEXT, optimize_scalarField_text, OPTIMIZE_SCALARFIELD_TEXTBOX_ID, control_cb);
    //optimize_scalarField_box->set_w(400);
    //optimize_scalarField_button = glui->add_button_to_panel(opt_rollout, "Optimize Scalar Field", OPTIMIZE_SCALARFIELD_ID, control_cb);

    //GLUI_Rollout *optn_debug_rollout = glui->add_rollout_to_panel(opt_rollout, "Debug", true);
    //optn_showOriginal_checkbox = glui->add_checkbox_to_panel(optn_debug_rollout, "Show Original Mesh", &gShowOriginalMesh, SHOW_ORIGINALMESH_ID, control_cb );
    //optn_opacity_spinner = glui->add_spinner_to_panel( optn_debug_rollout, "Opacity: ", GLUI_SPINNER_FLOAT, &optn_opacity, OPTN_OPACITY_ID, control_cb);
    //optn_opacity_spinner->set_float_limits( 0.0, 1.0 );
    //optn_opacity_spinner->set_speed( .03 );

    //GLUI_EditText* optimize_debug_textbox = glui->add_edittext_to_panel(optn_debug_rollout, "Debug:", GLUI_EDITTEXT_TEXT, optimize_debug_text, OPTIMIZE_DEBUG_TEXTBOX_ID, control_cb);
    //optimize_debug_textbox->set_w(400);
    //optimize_loadDebug_button = glui->add_button_to_panel(optn_debug_rollout, "Load Debug", OPTIMIZE_LOADDEBUG_BUTTON_ID, control_cb);
    //optimize_saveDebug_button = glui->add_button_to_panel(optn_debug_rollout, "Save Debug", OPTIMIZE_SAVEDEBUG_BUTTON_ID, control_cb);

    ///***** Marching Cube controls *****/
    //GLUI_Rollout *mc_rollout = glui->add_rollout( "Marching Cube" , true);

    ////marchingCube_button = glui->add_button_to_panel(mc_rollout, "*DEBUG ONLY* - Marching Cube on Sphere", MARCHING_CUBE_BUTTON_ID, control_cb);

    //open_scalarField_box = glui->add_edittext_to_panel(mc_rollout, "Open Scalar Field:", GLUI_EDITTEXT_TEXT, open_scalarField_text, OPEN_SCALARFIELD_TEXTBOX_ID, control_cb);
    //open_scalarField_box->set_w(400);

    //GLUI_Panel *opensf_panel = glui->add_panel_to_panel(mc_rollout, "");
    //open_sf_midpoint_button = glui->add_button_to_panel(opensf_panel, "Mid-point", OPEN_SF_MIDPOINT_ID, control_cb);
    //open_sf_interp_button = glui->add_button_to_panel(opensf_panel, "Interpolated", OPEN_SF_INTERP_ID, control_cb);
    //glui->add_column_to_panel(opensf_panel, false);
    //open_sf_no_interp_button = glui->add_button_to_panel(opensf_panel, "Nonobtuse-interpolated", OPEN_SF_NO_INTERP_ID, control_cb);
    //tiling_nonobtuse_button = glui->add_button_to_panel(opensf_panel, "Tiling Nonobtuse", TILING_NONOBTUSE_ID, control_cb);
    //simple_MC_button = glui->add_button_to_panel(opensf_panel, "Simple MC", SIMPLE_MC_ID, control_cb);

    //showScalarField_checkbox = glui->add_checkbox_to_panel(mc_rollout, "Show Scalar Field *DEBUG ONLY*", &gShowScalarField, SHOW_SCALARFIELD_ID, control_cb );

    //GLUI_Rollout *mc_spinner_rollout = glui->add_rollout_to_panel(mc_rollout, "Scalar Field Parameters");

    //nCellsX_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "numCell X:    ", GLUI_SPINNER_INT, &nCellX, NCELLX_ID, control_cb);
    //nCellsX_spinner->set_int_limits( 0, 10000 );
    //nCellsX_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //nCellsY_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "numCell Y:    ", GLUI_SPINNER_INT, &nCellY, NCELLY_ID, control_cb);
    //nCellsY_spinner->set_int_limits( 0, 10000 );
    //nCellsY_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //nCellsZ_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "numCell Z:    ", GLUI_SPINNER_INT, &nCellZ, NCELLZ_ID, control_cb);
    //nCellsZ_spinner->set_int_limits( 0, 10000 );
    //nCellsZ_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //showCellsRegion_checkbox = glui->add_checkbox_to_panel(mc_spinner_rollout, "Show Cells Region", &gShowCellsRegion, SHOW_CELLREGION_ID, control_cb );
    //nCellsZ_spinner->set_alignment( GLUI_ALIGN_LEFT );

    //glui->add_column_to_panel(mc_spinner_rollout, true);

    //fCellLengthX_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "Cell Length X:    ", GLUI_SPINNER_FLOAT, &fCellLengthX, FCELLLENGTHX_ID, control_cb);
    //fCellLengthX_spinner->set_float_limits( 0.0001, 10000.0 );
    //fCellLengthX_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //fCellLengthY_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "Cell Length Y:    ", GLUI_SPINNER_FLOAT, &fCellLengthY, FCELLLENGTHY_ID, control_cb);
    //fCellLengthY_spinner->set_float_limits( 0.0001, 10000.0 );
    //fCellLengthY_spinner->set_alignment( GLUI_ALIGN_LEFT );
    //fCellLengthZ_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "Cell Length Z:    ", GLUI_SPINNER_FLOAT, &fCellLengthZ, FCELLLENGTHZ_ID, control_cb);
    //fCellLengthZ_spinner->set_float_limits( 0.0001, 10000.0 );
    //fCellLengthZ_spinner->set_alignment( GLUI_ALIGN_LEFT );

    //fIsoValue_spinner = glui->add_spinner_to_panel(mc_spinner_rollout, "IsoValue:    ", GLUI_SPINNER_FLOAT, &fIsoValue, FISOVALUE_ID, control_cb);
    //fIsoValue_spinner->set_float_limits( -100.0, 100.0 );
    //fIsoValue_spinner->set_alignment( GLUI_ALIGN_LEFT );

    //save_scalarField_box = glui->add_edittext_to_panel(mc_rollout, "Save Scalar Field:", GLUI_EDITTEXT_TEXT, save_scalarField_text, SAVE_SCALARFIELD_TEXTBOX_ID, control_cb);
    //save_scalarField_box->set_w(400);
    //save_scalarField_button = glui->add_button_to_panel(mc_rollout, "Save", SAVE_SCALARFIELD_ID, control_cb);


    ///***** Navigation controls *****/
    //GLUI_Rollout *panel_nav = glui->add_rollout( "Navigation controls" , true);

    //glui->add_statictext_to_panel(panel_nav, "       object 1");
    //glui->add_separator_to_panel(panel_nav);
    //view_rot_obj = glui->add_rotation_to_panel( panel_nav, "Rotation", obj_rotate, NAV_ID, control_cb);
    //view_rot_obj->set_spin( 0.001 );

    //trans_xy_obj = glui->add_translation_to_panel( panel_nav, "Translate XY", GLUI_TRANSLATION_XY, obj_pos, NAV_ID, control_cb);
    //trans_xy_obj->set_speed( .0001 );

    //trans_z_obj = glui->add_translation_to_panel( panel_nav, "Translate Z", GLUI_TRANSLATION_Z, &obj_pos[2], NAV_ID, control_cb);
    //trans_z_obj->set_speed( .0001 );

    //glui->add_button_to_panel( panel_nav, "reset", RESET_OBJ_ID, control_cb );

    //glui->add_column_to_panel(panel_nav, true);

    //glui->add_statictext_to_panel(panel_nav, "       light");
    //glui->add_separator_to_panel(panel_nav);
    //view_rot_light = glui->add_rotation_to_panel( panel_nav, "Rotation", light_rotate, LIGHT_NAV_ID, control_cb);
    //view_rot_light->set_spin( 0.001 );

    //glui->add_button_to_panel( panel_nav, "reset", RESET_LIGHT_ID, control_cb );

    //glui->add_column_to_panel(panel_nav, true);

    //glui->add_statictext_to_panel(panel_nav, "       scene");
    //glui->add_separator_to_panel(panel_nav);
    //view_rot_scene = glui->add_rotation_to_panel( panel_nav, "Rotation", scene_rotate, NAV_ID, control_cb);
    //view_rot_scene->set_spin( 0.001 );

    //trans_xy_scene = glui->add_translation_to_panel( panel_nav, "Translate XY", GLUI_TRANSLATION_XY, scene_pos, NAV_ID, control_cb);
    //trans_xy_scene->set_speed( .0001 );

    //trans_z_scene = glui->add_translation_to_panel( panel_nav, "Zoom", GLUI_TRANSLATION_Z, &scene_pos[2], NAV_ID, control_cb);
    //trans_z_scene->set_speed( .0001 );
    //glui->add_button_to_panel( panel_nav, "reset", RESET_SCENE_ID, control_cb );


    ///****** 'open', 'save', 'quit' buttons *****/
    //GLUI_Rollout *file_rollout = glui->add_rollout( "File" , false);

    //open_text_box = glui->add_edittext_to_panel(file_rollout, "Open Mesh:", GLUI_EDITTEXT_TEXT, open_text, OPEN_TEXTBOX_ID, control_cb);
    //open_text_box->set_w(400);
    //open_button = glui->add_button_to_panel(file_rollout, "Open", OPEN_BUTTON_ID, control_cb);
    //save_text_box = glui->add_edittext_to_panel(file_rollout, "Save Mesh:", GLUI_EDITTEXT_TEXT, save_text, SAVE_TEXTBOX_ID, control_cb);
    //save_text_box->set_w(400);
    //save_button = glui->add_button_to_panel(file_rollout, "Save", SAVE_BUTTON_ID, control_cb);

    //glui->add_separator();  
    //glui->add_button( "Quit", 0,(GLUI_Update_CB)exit );

    ///**** Link windows to GLUI, and register idle callback ******/
    //glui->set_main_gfx_window( main_window );

    ///**** We register the idle callback with GLUI, *not* with GLUT ****/
    //GLUI_Master.set_glutIdleFunc( myGlutIdle );
}

void glut_init(int argc, char* argv[])
{
    // ***************************************
    // *  Initialize GLUT and create window  *
    // ***************************************
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowPosition( 30, 30 );
    glutInitWindowSize( 900, 900 );

    main_window = glutCreateWindow( "John Yung San Li - Nonobtuse Triangulation" );
    glutDisplayFunc( myGlutDisplay );
    glutReshapeFunc( myGlutReshape );
    glutMouseFunc( myGlutMouse );
    glutMotionFunc( myGlutMotion );
    glutKeyboardFunc( myGlutKeyboard );
    glutVisibilityFunc( myGlutVisibility);

    glColor3f(0.0, 0.0, 0.0); // wireframe colour

    // ****************************************
    // *       Set up OpenGL lights           *
    // ****************************************

    glEnable(GL_LIGHTING);
    glEnable( GL_NORMALIZE );

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    // ****************************************
    // *          Enable z-buferring          *
    // ****************************************

    glEnable(GL_DEPTH_TEST);

    // ****************************************
    // *         Enable Polygon Offset        *
    // ****************************************

    glEnable(GL_POLYGON_OFFSET_LINE);

    // ****************************************
    // *          Set Shading                 *
    // ****************************************

    glShadeModel(GL_SMOOTH);

    // ****************************************
    // *          Enable Blending             *
    // ****************************************

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/********** init() ***************/

bool init(int argc, char* argv[])
{
    centroid = new double[3];

    switch (argc)
    {
    case 1:		// no arguments: {program_name}
        glut_init(argc, argv);
        glui_init();

        // try to load file
        if (strcmp(open_text, "") != 0)
        {
            resetProperties();  // reset feature threshold before load file
            if (!g_rdr.loadFile(open_text))
            {
                // can't load file
                cout << "Failed loading file: " << open_text << endl;
            }
            else
            {
                // file loaded
                updateObtuseAngles();
                //g_optn.reset();
            }
        }
        break;

    case 2:		// load mesh: {program_name} {mesh_filename}
        // initialize GLUT and GLUI code
        glut_init(argc, argv);
        glui_init();
        if (g_rdr.loadFile(argv[1]))	// o.w. load mesh
            updateObtuseAngles();
        else
            cout << "Failed to load mesh file: " << argv[2] << endl;
        break;
    case 10:
        /*	
        // decimate debug file: {program_name} -decimateDebug {input_debug_file} {output_mesh_file} {num_Remaining} {decimate_threshold} {angleBound} {avoid_degree} {optn_alpha} {haveAngleBound}
        if (strcmp(argv[1], "-decimateDebug") == 0)	// decimate
        {
            g_region = SIMPLE3PLANES_TOWARD_CENTER;
            if (atoi(argv[9]) != 0)
                g_region = ANGLEBOUND_7PLANES;

            // perform smoothing then optimization
            g_optn.loadDebug(argv[2]);
            // decimate
            g_optn.decimate(atoi(argv[4]), atof(argv[5]), atof(argv[6]), atoi(argv[7]), atof(argv[8]), g_region);
            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), 
                g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
            g_rdr.saveFile(argv[3]);
        }
        // generate scalar field: {program_name} -s {mesh_filename} {scalarField_filename} {nCellsX} {nCellsY} {nCellsZ} {fCellLengthX} {fCellLengthY} {fCellLengthZ}
        else if (strcmp(argv[1], "-s") == 0)	// save scalar field
        {
            if (!g_rdr.loadFile(argv[2]))
            {
                cout << "Failed to load mesh file: " << argv[2] << endl;
            }
            else
            {	
                // check if mesh is open
                vector<int> holesOneRingList;
                unsigned int numHoles = g_nonObtuse.getHoles(g_rdr.getVertices(), g_rdr.getNumVertices(), 
                    g_rdr.getPolygons(), g_rdr.getNumPolygons(), 
                    g_rdr.getEdgeNeighbourList(), holesOneRingList);

                if (numHoles > 0)	// open meshes
                {
                    double* newVList = NULL;
                    int numNewVertices = 0;
                    int ** newPList = NULL;
                    int numNewPolygons = 0;

                    // fill up hole
                    if (g_nonObtuse.fillHoles(g_rdr.getVertices(), g_rdr.getNumVertices(), 
                        g_rdr.getPolygons(), g_rdr.getNumPolygons(), 
                        holesOneRingList, numHoles,
                        newVList, numNewVertices,
                        newPList, numNewPolygons))
                    {
                        // write scalar field
                        if (!g_nonObtuse.writeScalarField(newVList, numNewVertices, newPList, numNewPolygons, argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atof(argv[8]), atof(argv[9]), g_rdr.getNumPolygons()))
                            cout << "Failed writing scalar field file: " << save_scalarField_text << endl;
                    }
                    else
                    {
                        cout << "Failed filling holes in open mesh." << endl;
                    }

                    // clean up
                    delete[] newVList;
                    for (int i = 0; i < numNewPolygons; ++i)
                        delete[] newPList[i];
                    delete[] newPList;
                }
                else 				// closed meshes
                {
                    if (!g_nonObtuse.writeScalarField(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atof(argv[7]), atof(argv[8]), atof(argv[9])))
                        cout << "Failed writing scalar field file: " << argv[3] << endl;
                }
            }
        }
        return false;
        break;
    case 11:
        if (strcmp(argv[1], "-MC") == 0)	// initial mesh: {program_name} -MC {input_mesh_file} {output_mesh_file} {fIsoValue} {nCellsX} {nCellsY} {nCellsZ} {fCellLengthX} {fCellLengthY} {fCellLengthZ}
        {
            g_rdr.loadFile(argv[2]);

            if (g_rdr.isLoaded())
            {
                g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atof(argv[8]), atof(argv[9]), atof(argv[10]));

                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_tilingNOMC.getVertices(novList);
                int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

                // copy vertices back to renderer
                g_rdr.loadMeshInfo(novList, numNoVertices, nopList, numNoPolygons);			
                g_rdr.saveFile(argv[3]);
            }
        }
        // decimate file: {program_name} -decimate {input_mesh_file} {input_nonobtuse_mesh_file} {output_decimated_mesh_file} {num_Remaining} {decimate_threshold} {angleBound} {avoid_degree} {optn_alpha} {haveAngleBound}
        else if (strcmp(argv[1], "-decimate") == 0)
        {
            g_region = SIMPLE3PLANES_TOWARD_CENTER;
            if (atoi(argv[10]) != 0)
                g_region = ANGLEBOUND_7PLANES;

            // load input nonobtuse mesh
            g_rdr.loadFile(argv[3]);
            double* novList = NULL;
            int** nopList = NULL;
            int numNoVertices = g_rdr.getVertices(novList);
            int numNoPolygons = g_rdr.getPolygons_idx0(nopList);

            // load input mesh
            g_rdr.loadFile(argv[2]);

            g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

            // decimate
            //g_optn.decimate(atoi(argv[5]), atof(argv[6]), atof(argv[7]), atoi(argv[8]), atof(argv[9]), g_region);
            g_optn.decimate_MC_LinearSearch(atoi(argv[5]), atof(argv[6]), atof(argv[7]), atoi(argv[8]), atof(argv[9]));

            // copy vertices back to renderer
            g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
            g_rdr.saveFile(argv[4]);
        }
        return false;
        break;
    case 15:	
        g_region = SIMPLE3PLANES_TOWARD_CENTER;
        if (atoi(argv[8]) != 0)
            g_region = ANGLEBOUND_7PLANES;

        if (strcmp(argv[1], "-so") == 0)	// generate optimization debug file: {program_name} -so {input_debug_file} {output_debug_file} {num_Iter} {threshold} {improvementThreshold} {alpha} {numOneRingSearch} {numOneRingQuadric} {angleBound} {bRecomputeQuadric} {bRecomputeClosestFace} {haveAngleBound} {CleanWhenStuck}
        {
            // perform smoothing then optimization
            g_optn.loadDebug(argv[2]);
            g_optn.optimize_smoothing(atoi(argv[4]), atof(argv[5]), atoi(argv[9]));
            g_optn.optimize_afterMove(atoi(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]), atoi(argv[8]), atoi(argv[9]), atof(argv[10]), atoi(argv[11]), atoi(argv[12]), atoi(argv[13]), g_region);
            g_optn.saveDebug(argv[3]);
        }
        else if (strcmp(argv[1], "-o") == 0)	// generate optimization debug file: {program_name} -o {input_debug_file} {output_debug_file} {num_Iter} {threshold} {improvementThreshold} {alpha} {numOneRingSearch} {numOneRingQuadric} {angleBound} {bRecomputeQuadric} {bRecomputeClosestFace} {haveAngleBound} {CleanWhenStuck}
        {
            // perform optimization
            g_optn.loadDebug(argv[2]);
            g_optn.optimize_afterMove(atoi(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]), atoi(argv[8]), atoi(argv[9]), atof(argv[10]), atoi(argv[11]), atoi(argv[12]), atoi(argv[15]), g_region);
            g_optn.saveDebug(argv[3]);
        }
        return false;
        break;

    case 16:
        g_region = SIMPLE3PLANES_TOWARD_CENTER;
        if (atoi(argv[8]) != 0)
            g_region = ANGLEBOUND_7PLANES;

        if (strcmp(argv[1], "-lo") == 0)	// generate optimization debug file: {program_name} -lo {input_mesh_file} {input_scalarfield_file} {output_debug_file} {num_Iter} {threshold} {improvementThreshold} {alpha} {numOneRingSearch} {numOneRingQuadric} {angleBound} {bRecomputeQuadric} {bRecomputeClosestFace} {haveAngleBound} {CleanWhenStuck}
        {
            if (strcmp(argv[3], "") != 0 && g_isoSurface.GenerateSurface(argv[3], 0, MIDPOINT))
            {
                g_rdr.loadFile(argv[2]);
                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_isoSurface.getVertices(novList);
                int numNoPolygons = g_isoSurface.getTriangles(nopList);

                g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

                // free up g_isoSurface
                g_isoSurface.DeleteSurface();

                // remove bad valences
                g_optn.removeBadValence(g_region, optn_angleBound);

                // optimize
                g_optn.optimize_afterMove(atoi(argv[5]), atof(argv[6]), atof(argv[7]), atof(argv[8]), atoi(argv[9]), atoi(argv[10]), atof(argv[11]), atoi(argv[12]), atoi(argv[13]), atoi(argv[15]), g_region);

                g_optn.saveDebug(argv[4]);
            }
        }
        return false;
        break;
    case 21:
        if (strcmp(argv[1], "-optimize") == 0)	// optimize mesh: {program_name} -optimize {input_mesh_file} {output_mesh_file} {fIsoValue} {nCellsX} {nCellsY} {nCellsZ} {fCellLengthX} {fCellLengthY} {fCellLengthZ} {num_Iter} {threshold} {improvementThreshold} {alpha} {numOneRingSearch} {numOneRingQuadric} {angleBound}  {bRecomputeClosestFace} {haveAngleBound} {numSmoothingStep}
        {
            if (strcmp(argv[2], "") != 0 && strcmp(argv[3], "") != 0)
            {
                g_region = SIMPLE3PLANES_TOWARD_CENTER;
                if (atoi(argv[19]) != 0)
                    g_region = ANGLEBOUND_7PLANES;

                g_rdr.loadFile(argv[2]);

                clock_t timeTaken = g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atof(argv[8]), atof(argv[9]), atof(argv[10]));
                // 					clock_t timeTaken = g_tilingNOMC.GenerateSurface_simpleMC(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atof(argv[8]), atof(argv[9]), atof(argv[10]));

                vector<clock_t> timeTakenList;
                timeTakenList.push_back(timeTaken);

                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_tilingNOMC.getVertices(novList);
                int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

                g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons, g_tilingNOMC.getClosestTri());

                // remove bad valences
                g_optn.removeBadValence(g_region, optn_angleBound);

                char savePath[100];
                strcpy(savePath, argv[3]);
                strcat(savePath, ".initial.smf");
                g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
                g_rdr.saveFile(savePath);

                // free up g_tilingNOMC
                g_tilingNOMC.DeleteSurface();

                // optimize in alternate fashion
                int numSmoothingStep = atoi(argv[20]);
                for (int i = 0; i < numSmoothingStep; ++i)
                {
                    timeTaken = g_optn.optimize_smoothing(atoi(argv[11]), atof(argv[12]), atof(argv[17]), atoi(argv[18]), g_region);
                    timeTakenList.push_back(timeTaken);

                    stringstream ss1;
                    ss1 << argv[3] << ".smooth." << i << ".smf";
                    g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
                    g_rdr.saveFile(ss1.str().c_str());

                    // 						if (i == numSmoothingStep-1)
                    // 							break;
                    // 						timeTaken = g_optn.optimize_afterMove(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), false, g_region);
                    // 						timeTaken = g_optn.optimize_afterMove_noPriority(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), false, g_region);
                    timeTaken = g_optn.optimize_afterMove(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), true, g_region);
                    // 						timeTaken = g_optn.optimize_afterMove_MCA(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), true, g_region);
                    timeTakenList.push_back(timeTaken);

                    stringstream ss2;
                    ss2 << argv[3] << ".optn." << i << ".smf";
                    g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());
                    g_rdr.saveFile(ss2.str().c_str());

                    // 						timeTaken = g_optn.optimize_smoothing(atoi(argv[11]), atof(argv[12]), atof(argv[17]), atoi(argv[18]), g_region);
                    // 						timeTakenList.push_back(timeTaken);
                }

                // 					timeTaken = g_optn.optimize_afterMove(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), true, g_region);
                //   					timeTaken = g_optn.optimize_afterMove_noPriority(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), true, g_region);
                // 						timeTaken = g_optn.optimize_afterMove_MCA(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), true, atoi(argv[18]), true, g_region);
                //   					timeTakenList.push_back(timeTaken);

                timeTaken = 0;
                cout << "Time taken in each step: ";
                for (unsigned int i = 0; i < timeTakenList.size(); ++i) {
                    cout << timeTakenList[i];
                    timeTaken += timeTakenList[i];
                    if (i < timeTakenList.size()-1)
                        cout << "secs, ";
                }
                cout << endl << "Total time: " << timeTaken << endl;

                // copy vertices back to renderer
                g_rdr.loadMeshInfo(g_optn.getNoVertices(), g_optn.getNumNoVertices(), g_optn.getNoPolygons(), g_optn.getNumNoPolygons());

                //g_rdr.saveFile(argv[3]);
                strcpy(savePath, argv[3]);
                strcat(savePath, ".final.smf");
                g_rdr.saveFile(savePath);

                // 					strcpy(savePath, argv[3]);
                // 					strcat(savePath, ".debug");
                // 
                // 					g_optn.saveDebug(savePath);
            }
        }
        return false;
        break;
    case 22:
        if (strcmp(argv[1], "-lonew") == 0)	// generate optimization debug file: {program_name} -lo {input_mesh_file} {output_debug_file} {fIsoValue} {nCellsX} {nCellsY} {nCellsZ} {fCellLengthX} {fCellLengthY} {fCellLengthZ} {num_Iter} {threshold} {improvementThreshold} {alpha} {numOneRingSearch} {numOneRingQuadric} {angleBound} {bRecomputeQuadric} {bRecomputeClosestFace} {haveAngleBound} {CleanWhenStuck}
        {
            if (strcmp(argv[2], "") != 0)
            {
                g_region = SIMPLE3PLANES_TOWARD_CENTER;
                if (atoi(argv[20]) != 0)
                    g_region = ANGLEBOUND_7PLANES;

                g_rdr.loadFile(argv[2]);

                g_tilingNOMC.GenerateSurface(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getPolygonNeighbourList(), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atof(argv[8]), atof(argv[9]), atof(argv[10]));

                double* novList = NULL;
                int** nopList = NULL;
                int numNoVertices = g_tilingNOMC.getVertices(novList);
                int numNoPolygons = g_tilingNOMC.getTriangles(nopList);

                g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons, g_tilingNOMC.getClosestTri());
                //g_optn.loadMesh(g_rdr.getVertices(), g_rdr.getNumVertices(), g_rdr.getPolygons(), g_rdr.getNumPolygons(), g_rdr.getVertexNeighbourList(), g_rdr.getEdgeNeighbourList(), g_rdr.getPolygonNeighbourList(), novList, numNoVertices, nopList, numNoPolygons);

                // free up g_isoSurface
                g_isoSurface.DeleteSurface();

                // remove bad valences
                g_optn.removeBadValence(g_region, optn_angleBound);

                // optimize
                g_optn.optimize_afterMove(atoi(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atoi(argv[15]), atoi(argv[16]), atof(argv[17]), atoi(argv[18]), atoi(argv[19]), atoi(argv[21]), g_region);

                g_optn.saveDebug(argv[3]);
            }
        }
        return false;
        break;
        */
    default:
        break;
    }

    return true;	// start GUI
}

int testori(int argc, char* argv[])
{	

    // **** Initialization ****

    if (!init(argc, argv))
        return 0;

    // **** Regular GLUT main loop ****

    glutMainLoop();

    // clean up
    delete[] centroid;

    return 0;
}

