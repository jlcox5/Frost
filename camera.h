/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the camera.
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

// Imported Libraries
#include "Eigen/Dense"
#include "Eigen/Geometry"

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"

using namespace Eigen;

class Camera{
  public:
    Camera();
    Camera(Camera const &);
    ~Camera();
    Camera& operator=(Camera const &);

    void setCamera() const;
    void setCameraHud() const;
    void rotateCamera(int);
    //void rotateCameraMouse(float, float);
    void translateCamera(int);

    Vector3f const & getAim() const{ return aim; }
    Vector3f const & getEye() const{ return eye; }

    void printCamera() const;
    void drawAimPoint() const;

  private:
    Vector3f eye;
    Vector3f aim;
    Vector3f up;
    float fov;
    float aspect_x;
    float aspect_y;
    float near_clip;
    float far_clip;
    
};

#endif
