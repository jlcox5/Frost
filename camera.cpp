/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the camera.
 *
 */

//SDL Libraries
#include <SDL_opengl.h>

// Custom Libraries
#include "camera.h"
#include "parseXML.h"
#include "client.h"

Camera::Camera(){

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();

  parser->parseXML("xml/camera.xml", handle);
  eye << handle->getXmlFloat("pos_x"), 
      handle->getXmlFloat("pos_y"), 
      handle->getXmlFloat("pos_z");
  aim << handle->getXmlFloat("aim_x"), 
      handle->getXmlFloat("aim_y"), 
      handle->getXmlFloat("aim_z");
  up << handle->getXmlFloat("up_x"), 
     handle->getXmlFloat("up_y"), 
     handle->getXmlFloat("up_z");
  fov = handle->getXmlFloat("fov");
  aspect_x = handle->getXmlFloat("aspect_x");
  aspect_y = handle->getXmlFloat("aspect_y");
  near_clip = handle->getXmlFloat("near_clip");
  far_clip = handle->getXmlFloat("far_clip");
  
  cout << "Building camera!" << endl;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective( fov, aspect_x/aspect_y, near_clip, far_clip );
  gluLookAt( eye(0), eye(1), eye(2), aim(0), aim(1), aim(2), up(0), up(1), up(2) );

  glMatrixMode(GL_MODELVIEW);

  delete handle;
}

Camera::~Camera(){
  cout << "Goodbye Camera!" << endl;
}

void Camera::setCamera() const{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective( fov, aspect_x/aspect_y, near_clip, far_clip );
  gluLookAt( eye(0), eye(1), eye(2), aim(0), aim(1), aim(2), up(0), up(1), up(2) );

  glMatrixMode(GL_MODELVIEW);
}

void Camera::setCameraHud() const{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective( fov, aspect_x/aspect_y, near_clip, far_clip );
  gluLookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0);

  glMatrixMode(GL_MODELVIEW);
}

void Camera::rotateCamera(int i){
  float ang = 3.0*i;
  AngleAxis<float> a((ang*M_PI)/180.0, Vector3f::UnitY() );
  Matrix3f m;
  Vector3f v1, v2, v3;
  Vector3f eyeOld;
  float dist;

  eyeOld = eye;

  // Define axis of rotation
  m = AngleAxisf((0.0*M_PI)/180.0, Vector3f::UnitX())
       * AngleAxisf((ang*M_PI)/180.0, Vector3f::UnitY())
       * AngleAxisf((0.0*M_PI)/180.0, Vector3f::UnitZ());

  // Find vector from eye to aim and use as rotation point around origin
  v1 << aim - eye;
  dist = v1.norm();

  // Rotate point and find vector between the original point and the rotated point
  v2 << v1;
  v2 << m * v2;
  v2 << v2 - v1;

  // Apply the difference to the eye
  v3 << eye + v2;
  v3 << v3 - aim;
  v3.normalize();
  eye << dist * v3 + aim;
  eye(1) = eyeOld(1);

  setCamera();
}

void Camera::translateCamera(int dir){
  if(Client::getInstance()->getPlayer().getCanMove()){
    float dist;
    Vector3f v1, v2, v3;

    v1 << 1.0, 0.0, 0.0;
    v2 << aim - eye;
    v2.normalize();
    v1 << v2(0), 0.0, v2(2);
    v1.normalize();
    v2 << v1;

    v3 << 0.0, 1.0, 0.0;
    v3 << v3.cross(v1);
    v3.normalize();
    dist = 1.0;

    v1 << aim - eye;

    switch(dir){
      // A is pressed
      case 0:
        aim << aim + v2*dist;
        eye << aim - v1;
        break;

      // D is pressed
      case 1:
        aim << aim - v2*dist;
        eye << aim - v1;
        break;

      // Q is pressed
      case 2:
        aim << aim + v3*dist;
        eye << aim - v1;
        break;

      // E is pressed
      case 3:
        aim << aim - v3*dist;
        eye << aim - v1;
        break;
    }    

    setCamera();
  }
}

void Camera::printCamera() const{
  cout << "  Eye : " << eye(0) << " " << eye(1) << " " << eye(2) << endl;
  cout << "  Aim : " << aim(0) << " " << aim(1) << " " << aim(2) << endl;
  cout << "  Up : " << up(0) << " " << up(1) << " " << up(2) << endl;
  cout << "   Eye to Aim distance: " << (eye-aim).norm() << endl;
}

void Camera::drawAimPoint() const{
  glPointSize(4.0);
  glColor4f(1.0, 0.0, 1.0, 1.0);
  glBegin(GL_POINTS);
    glVertex3f(aim(0), aim(1), aim(2));
  glEnd();
}
