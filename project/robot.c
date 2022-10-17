#include "robot.h"

void setup_robot(struct Robot *robot){
    robot->x = OVERALL_WINDOW_WIDTH/2-50;
    robot->y = OVERALL_WINDOW_HEIGHT-50;
    
    robot->true_x = 117;
    robot->true_y = OVERALL_WINDOW_HEIGHT-100;
    
//    //for the curvy maze
//    robot->true_x = OVERALL_WINDOW_WIDTH/2-110;
//    robot->true_y = OVERALL_WINDOW_HEIGHT-90;
    
//    robot->true_x = OVERALL_WINDOW_WIDTH/2-50;
//    robot->true_y = OVERALL_WINDOW_HEIGHT-50;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 0;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorFrontCentreAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        //xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        //yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xDir = round(robotCentreX-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        //xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        //yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xDir = round(robotCentreX+(+ROBOT_WIDTH/2)*cos((robot->angle-90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle-90)*PI/180));
        yDir = round(robotCentreY+(+ROBOT_WIDTH/2)*sin((robot->angle-90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle-90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        //xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        //yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+90)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    /*
    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap
    SDL_Rect rect = {robot->x, robot->y, robot->height, robot->width};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    */
    /*
    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);
    */

    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);

    //Front Centre Sensor
    int sensor_sensitivity =  floor(SENSOR_VISION/5);
    int i;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(+ROBOT_WIDTH/2)*cos((robot->angle-90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle-90)*PI/180));
        yDir = round(robotCentreY+(+ROBOT_WIDTH/2)*sin((robot->angle-90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle-90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Right Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle+90)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle+90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

        //xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        //yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
}



void robotMotorMove(struct Robot * robot, int crashed) {
    double x_offset, y_offset;
    if (crashed)
        robot->currentSpeed = 0;
    else {
        switch(robot->direction){
            case UP :
                robot->currentSpeed += DEFAULT_SPEED_CHANGE;
                if (robot->currentSpeed > MAX_ROBOT_SPEED)
                    robot->currentSpeed = MAX_ROBOT_SPEED;
                break;
            case DOWN :
                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
                if (robot->currentSpeed < -MAX_ROBOT_SPEED)
                    robot->currentSpeed = -MAX_ROBOT_SPEED;
                break;
            case LEFT :
                robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
                break;
            case RIGHT :
                robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
                break;
        }
    }
    robot->direction = 0;
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;
}

void robotAutoMotorMove(struct Robot * robot, int front_centre_sensor, int left_sensor, int right_sensor, int right_prev, int left_prev, int randomBit)
{
    robot->currentSpeed = 0.15*MAX_ROBOT_SPEED;
    
    //printf("Right previous = %d\n" ,right_prev);
    //printf("Left previous = %d" ,left_prev);
    
    // int randomBit = rand() % 2;
    
    if (front_centre_sensor == 0 && right_sensor <= 3 && left_sensor <= 3 )
    {
        //if there is nothing in front and left and right are not dangerously close


        if (right_prev > 0 && right_sensor == 0)
        {
            //if there was something sensed by the right sensor before
            //but now there is nothing then turn right
         printf("no right wall if entered\n");
         robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
        robot->direction = RIGHT;
        }
        else if (left_prev > 0 && left_sensor == 0)
        {
            // if threre was something sensed on the left of the robot but its not
            // there anymore then turn left
            printf("Left wall lost\n");
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            robot->direction = LEFT;
        }
        else if (front_centre_sensor == 0 && right_sensor ==0 && left_sensor == 0)
        {
            printf("nothing on either sides or front\nvalue of rand = %d\n", randomBit);
            printf("value of left_prev = %d\n", left_prev);
            printf("value of right_prev = %d\n", right_prev);
            if(randomBit == 0)
            {
                printf("no sensor entered, dir - right\n");
                robot->direction = RIGHT;
                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

                // SDL_Delay(50);
                // robot->direction = UP;

            }
            else
            {
                if (randomBit == 1)
                {
//                    if (right_sensor <= 3 && left_sensor <= 3 )
                    printf("no sensor entered dir - left\n");
                    robot->direction = LEFT;
                    robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

                    // SDL_Delay(50);
                }
                else
                {
//                    if (right_sensor <= 3 && left_sensor <= 3 )
                    robot->direction = UP;

                }
            }
        }
        else if (front_centre_sensor == 0 && right_sensor >=3 && left_sensor <= 1 )
        {
            robot->direction = LEFT;
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

        }
        else if (front_centre_sensor == 0 && right_sensor <=1 && left_sensor >= 3 )
        {
            robot->direction = RIGHT;
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

        }
//
        else
        {
            if (right_sensor <= 3 && left_sensor <= 3 )
            robot->direction = UP;
        }
    }
    
    else 
    {
        if (front_centre_sensor > 0)
        {
            printf("Something in front\n");
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            
            if(right_sensor <= left_sensor)
            {
                printf("turning right");
                robot->direction = RIGHT;
                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

            }
//            else if (right_sensor == left_sensor)
//            {
//                printf("both sides equal keep turning in the same direction reached\n");
//                robot->direction = robot->direction;
//                printf("Current direction that is retained: %d", robot->direction);
//                //look into this
//            }
            
            
//            if (left_prev > 0 )
//            {
//                printf("turning right\n");
//                robot->direction = RIGHT;
//                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
//
////
//            }
//            else if (right_prev > 0)
//            {
//                printf("turning left");
//                robot->direction = LEFT;
//                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
//            }
            
//            else if (right_sensor < left_sensor)
//            {
//                printf("else case\n");
//                robot->direction = RIGHT;
//                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
//
//            }
            else if (left_sensor < right_sensor)
            {
                
                printf("else case ");
                printf("turning left");
                robot->direction = LEFT;
                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

            }
        }
        
//        else if (left_sensor > right_sensor)
//            {
//                robot->direction = RIGHT;
//                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
//
//            }
//        else if(right_sensor > left_sensor)
//            {
//                robot->direction = LEFT;
//                robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
//
//
//            }
        else
        {
             if (randomBit == 0)
                {
                    robot->direction = LEFT;
                    robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

                    
                }
                else
                {
                    robot->direction = RIGHT;
                    robot->currentSpeed -= DEFAULT_SPEED_CHANGE;

                }
        }

        //if left side is empty and something on the right side
        // if(left_sensor == 0 || right_sensor >=3)
        // {
            
        //     if (left_sensor > right_sensor)
        //     {
        //         robot->direction = RIGHT;
        //     }
        //     else if(right_sensor > left_sensor)
        //     {
        //         robot->direction = LEFT;
        //     }
        //     // robot->currentSpeed = 
        //     else
        //     {
        //         robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
        //         // robot->direction = LEFT;
        //         if (randomBit == 0)
        //         {
        //             robot->direction = LEFT;
                    
        //         }
        //         else
        //         {
        //             robot->direction = RIGHT;
        //         }
        //     }
            
        // }
        // //if nothing on the right or something on the left side
        // else
        // {   
        //     if (left_sensor > right_sensor)
        //     {
        //         robot->direction = RIGHT;
        //     }
        //     else if(right_sensor > left_sensor)
        //     {
        //         robot->direction = LEFT;
        //     }
        //     //if right side empty or something is on the left side then go right
        //     else
        //     {
        //         if(right_sensor == 0 || left_sensor >= 3)
        //     {
        //         robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
        //         robot->direction = RIGHT;
        //     }
        //     //if something is on all right left and forward then reverse
        //     else
        //     {
        //         robot->direction = DOWN;
        //     }
        //     }
        
    }
    }
    

