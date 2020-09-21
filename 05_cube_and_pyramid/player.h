
#include "vec3f.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct Player {

    float px;
    float py;
    float pz;

    Vec3f vx;
    Vec3f vy;
    Vec3f vz;

    Player() ;

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void MoveForward();
    void MoveBackward();

    void RotateUp();
    void RotateDown();
    void RotateLeft();
    void RotateRight();
    void RotateZLeft();
    void RotateZRight();
    
    glm::mat4 GetPlayerLookAt() const ;
};