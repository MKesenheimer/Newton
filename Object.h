#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

class Object {
  public:
    //the constructor
    Object(float x, float y, float vx = 0, float vy = 0, float size = 1, float angle = 0, float spin = 0);
    
    //gives the x and y position in the 2 dimensional world
    float x();
    float y();
    float xcenter();
    float ycenter();
    //gives the velocity in x- and y-direction of the object
    float vx();
    float vy();
    //get the angle in respect to the y-axis
    float phi();
    float spin();
    //gives the horizontal size
    float size();
    //get the number of points
    int npoints();
    
    //set the size
    void set_size(float size);
    
    //set the x and y position in the 2 dimensional world
    void set_pos(float x, float y);
    void set_v(float vx, float vy);
    
    //set the objects angle
    void set_angle(float angle);
    void set_spin(float spin);
    
    //update the new position of the Object
    void update_position(float dt);
    
    //save a new coordinate and remember the index. iscol is used to determine
    //if point should be used for collision control
    void new_point(float x, float y, bool iscol = true);
    std::vector<float> get_point(int n);
    bool is_collidable(int n);
    void modify_point(float x, float y, int n);

  private:
    float x_;
    float y_;
    float size_;
    float vx_;
    float vy_;
    float phi_; //angle in degree (°)
    float old_phi_; 
    //whenever save_point() is called, we increment this numbers
    int npoints_;
    struct point {
        int index;
        float x;
        float y;
        bool iscollidable;
    };
    float spin_;
    std::vector<point> points; //points are defined in the object coordinate system
};

#endif