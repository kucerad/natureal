#include "Camera.h"

GLuint edges[] = {0,1, 2,3, 4,5, 6,7, 0,2,2,4,4,6,6,0, 1,3,3,5,5,7,7,1};

Camera::Camera() {
	activityFactor = 0.f;
	human_movement = v3(0.f);
	step = 0.5f;
	terrain = NULL;
	corners[0] = Vector4(-1.0f, -1.0f, -1.0f, 1.0f); //left, bottom, near
	corners[1] = Vector4(-1.0f, -1.0f,  1.0f, 1.0f); //left, bottom, far
	corners[2] = Vector4(-1.0f,  1.0f, -1.0f, 1.0f); //left, top, near
	corners[3] = Vector4(-1.0f,  1.0f,  1.0f, 1.0f); //left, top, far
	corners[4] = Vector4( 1.0f,  1.0f, -1.0f, 1.0f); //right, top, near
	corners[5] = Vector4( 1.0f,  1.0f,  1.0f, 1.0f); //right, top, far
	corners[6] = Vector4( 1.0f, -1.0f, -1.0f, 1.0f); //right, bottom, near
	corners[7] = Vector4( 1.0f, -1.0f,  1.0f, 1.0f); //right, bottom, far

	vbo = 0;
	index_vbo = 0;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &index_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);

	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24*sizeof(GLuint), edges, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Camera::set(Camera * _cam) {
	terrain  = _cam->terrain;
	step = _cam->step;
	position = _cam->position;
	direction = _cam->direction;
}

Camera::~Camera() {
   glDeleteBuffers(1,&vbo);
   glDeleteBuffers(1,&index_vbo);
}

void Camera::setPositon(float x, float y, float z) {
	position = v3(x, y, z);
}

void Camera::setPositon(v3 _v) {
	position = _v;
}

v3 Camera::getPosition(void) {
	return position+human_movement;
}

void Camera::setDirection(float x, float y, float z) {
	direction = v3(x, y, z);
	direction.normalize();
}

void Camera::setDirection(v3 _v) {
	direction = _v;
	direction.normalize();
}

v3 Camera::getDirection(void) {
	return direction;
}

bool Camera::handleKeyDown(int key, int action)
{
	v3 right = direction.cross(upVector);

	switch(key){
		case 'a':
		case 'A':
			move(right*-step);
			break;
		case 'd':
		case 'D':
			move( right*step);
			break;
		case 's':
		case 'S':
			move (direction*-step);
			break;
		case 'w':
		case 'W':
			move( direction*step);
			break;
		default:
			return false;		
	}
	return true;
}

void Camera::handleMouseMove(int x, int y)
{
	float dx = x - g_WinWidth/2;
	float dy = y - g_WinHeight/2;
	//printf("handle mouse CAMERA, dx:%f, dy:%f\n", dx,dy);
	
	v3 right = direction.cross(upVector);
	v3 top = v3(0.f, 1.f, 0.f);

	v3 axis = top*dx + right*dy;
	axis.normalize();
	float angle = v3(dx,dy,0).length()/100.0;
	rotate(axis, -angle);
}

m4 Camera::getViewMatrix() {
	m4 view;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	v3 p(position+human_movement);
	gluLookAt(p.x				, p.y				, p.z				,
			  p.x+direction.x	, p.y+direction.y	, p.z + direction.z	,
			  upVector.x		, upVector.y		, upVector.z		);

	glGetFloatv(GL_MODELVIEW_MATRIX, view); //obtaining view matrix
	glPopMatrix();
	return view;
}

m4 Camera::getProjectionMatrix() {
	m4 projection;
	glGetFloatv(GL_PROJECTION_MATRIX, projection); //obtaining projection matrix
	return projection;
}

void Camera::computeCorners() {
	m4 view, projection;

	view = getViewMatrix();				//obtaining view matrix
	projection = getProjectionMatrix(); //obtaining projection matrix

	//inverse both
	view.invert();
	projection.invert();

	//from post-projective space to world space
	for (int i=0; i<8; i++) {
		cornersCameraInWs[i] = (view * projection) * corners[i];
		cornersCameraInWs[i] = cornersCameraInWs[i]/cornersCameraInWs[i].w;
	}
}

Vector4 * Camera::getCornersCameraInWs() {
	return cornersCameraInWs;
}

void Camera::draw() {
	for (int i=0; i<8; i++) {
		points[i*3] = cornersCameraInWs[i].x;
		points[i*3+1] = cornersCameraInWs[i].y;
		points[i*3+2] = cornersCameraInWs[i].z;
	}

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);

   glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), points, GL_STREAM_DRAW);

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, NULL);
	
   glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

   glDisableClientState(GL_VERTEX_ARRAY);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Camera::shoot()
{
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, g_WinWidth, g_WinHeight);
  gluPerspective(fov, (GLfloat)g_WinWidth/g_WinHeight, near, far);  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  v3 p(position+human_movement);
  gluLookAt(p.x				, p.y				, p.z				,
			p.x+direction.x	, p.y+direction.y	, p.z + direction.z	,
			upVector.x		, upVector.y		, upVector.z		);

}

void Camera::setup(v3 & pos, v3 & dir, v3 &up, int *w, int *h, float fo, float n, float fa)
{
	position	= pos;
	direction	= dir;
	upVector	= up;
	width		= w;
	height		= h;
	fov			= fo;
	near		= n;
	far			= fa;
}

void Camera::move(v3 & dist)
{
	position = position + dist;
	
	if (terrain!=NULL && mode!=FREE){
		float x = position.x +terrain->sz_x/2.0;
		float z = position.z +terrain->sz_y/2.0;
		switch (mode){
		case TERRAIN_RESTRICTED:
			position.y = max(terrain->getHeightAt(x, z)+HUMAN_HEIGHT, position.y);
			break;
		case TERRAIN_CONNECTED:
			position.y = terrain->getHeightAt(x, z)+HUMAN_HEIGHT;
			break;
		case WALK:
			activityFactor += HUMAN_ACTIVITY_INCR;
			activityFactor = min(activityFactor, HUMAN_MAX_ACTIVITY);
			position.y = terrain->getHeightAt(x,z)+HUMAN_HEIGHT;

		}

	}
	// restrict to the world cube
}

void Camera::rotate(v3 & axis, float angle)
{
	static const v3 up(0.f, 1.f, 0.f);
	static const v3 dir(0.f, 0.f, -1.f);
	direction.rotate(angle, axis);
	direction.normalize();
	//float angle2 = direction.angleTo(dir);
	//v3 axis2 = direction.cross(dir);

	// recalculate up-vector & right-vector
	//if (axis2.length()>= 0.01){
		//upVector = up.getRotated(angle2, axis2);
	//} else {
		//upVector = up;
	//}
}

void Camera::zoom(float fovy)
{
	fov = fovy;
}

void Camera::setTerrain(Terrain * terr)
{
	terrain = terr;
}

void Camera::setMode(CameraMode m)
{
	mode = m;
}

void Camera::update(double _time){
	time = _time;
	switch (g_cameraMode){
		case WALK:
		// human is calming down...
		activityFactor *= HUMAN_ACTIVITY_DECAY;
		activityFactor = max(activityFactor, HUMAN_MIN_ACTIVITY);
		// calc human movement
		human_movement.y = HUMAN_BREATH_AMPL*sin(time*activityFactor*HUMAN_BREATH_FREQ);
		//printf("act: %f \n", activityFactor);
		break;
	}
}
