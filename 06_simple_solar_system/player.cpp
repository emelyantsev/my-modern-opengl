#include "player.h"

#include <iostream>

Player::Player() : px(0.0f), py(0.0f), pz(8.0f), vx({1.0f, 0.0f, 0.0f}), vy({0.0f, 1.0f, 0.0f}), vz({0.0f, 0.0f, 1.0f}) {}


void Player::MoveUp() {
    px += vy[0];
    py += vy[1];
    pz += vy[2];
};

void Player::MoveDown() {
    px -= vy[0];
    py -= vy[1];
    pz -= vy[2];
};

void Player::MoveLeft() {
    px -= vx[0];
    py -= vx[1];
    pz -= vx[2];
};

void Player::MoveRight() {
    px += vx[0];
    py += vx[1];
    pz += vx[2];
};

void Player::MoveForward() {
    px -= vz[0];
    py -= vz[1];
    pz -= vz[2];
};

void Player::MoveBackward() {
    px += vz[0];
    py += vz[1];
    pz += vz[2];
};


void Player::RotateUp() {
    vy = rotate(vy, vx, -1) ;
    vz = rotate(vz, vx, -1) ;
};

void Player::RotateDown() {
    vy = rotate(vy, vx, 1) ;
    vz = rotate(vz, vx, 1) ;
};

void Player::RotateLeft() {
    vx = rotate(vx, vy, -1) ;
    vz = rotate(vz, vy, -1) ;

};

void Player::RotateRight() {
    vx = rotate(vx, vy, 1) ;
    vz = rotate(vz, vy, 1) ;
};

void Player::RotateZLeft() {
    vx = rotate(vx, vz, -1) ;
    vy = rotate(vy, vz, -1) ;

};

void Player::RotateZRight() {
    vx = rotate(vx, vz, 1) ;
    vy = rotate(vy, vz, 1) ;
};





glm::mat4 Player::GetPlayerLookAt() const {

    return glm::lookAt(glm::vec3(px, py, pz), glm::vec3(px-vz[0], py-vz[1], pz-vz[2]), glm::vec3(vy[0], vy[1], vy[2]) );
}