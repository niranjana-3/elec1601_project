void robotAutoMotorMove(struct Robot * robot, int front_centre_sensor, int left_sensor, int right_sensor, int right_prev, int left_prev) {
    robot->currentSpeed = 2;
    printf("Right previous = %d\n" ,right_prev);
    printf("Left previous = %d" ,left_prev);
    
    if (right_prev >= 1 && right_sensor == 0)
    {
        printf("no right wall if entered\n");
        robot->direction = RIGHT;
        }
    else if (left_prev >= 1 && left_sensor == 0)
        {
            robot->direction = LEFT;
        }

    if (front_centre_sensor == 0 && right_sensor <= 2 && left_sensor <= 2) {
        // robot->currentSpeed += DEFAULT_SPEED_CHANGE;
        //         if (robot->currentSpeed > MAX_ROBOT_SPEED)
        //             robot->currentSpeed = MAX_ROBOT_SPEED;
        robot->direction = UP;
        
    
    }
    else 
    {
        //if left side is empty and something on the right side
        if(left_sensor == 0 || right_sensor >=3)
        {
            // robot->currentSpeed = 
            robot->direction = LEFT;
            
        }
        //if nothing on the right or something on the left side
        else
        {
            //if right side empty or something is on the left side then go right
            if(right_sensor == 0 || left_sensor >= 3)
            {
                robot->direction = RIGHT;
            }
            //if something is on all right left and forward then reverse
            else
            {
                robot->direction = DOWN;
            }
        }
    }
