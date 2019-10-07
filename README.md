# Pioneer_3-DX_Simulation

### **Part C**

Users can manipulate the robot with the keyboard by hitting four different keys in an intuitive way:

**UP** - the robot moves a step forward \
**DOWN** - the robot moves a step backward \
**LEFT** - the robot rotates counterclockwise by a small angle \
**RIGHT** - the robot rotates clockwise by a small angle \

Note that when the robot approaches an obstacle, the robot moves by a smaller step or even stands still. \
This feature is designed to avoid the robot from collision.

--

### **Part E**

Given a target pose *(X, Y, θ)*, the robot moves from the current pose *(0, 0, 0)* to the target pose directly in a straight path.
This assumes that there are no obstacles within the direct path. \
The movement can be seperated into the following three steps.

1. Rotate to the orientation facing the target. 
2. After the rotation in step 1 is completed, translate straight to the position of the target (X, Y).
3. After the translation in step 2 is completed, rotate to the targeted orientation θ.

--

### **Bonus**

Similar as part E, but obstacles are allowed in this case.

Given a target (X, Y, θ), the robot tries to move to the target pose by checking the direct straight path in every ‘round’. If the robot cannot reach the target within the direct path, it moves only part of the available distance, make an ‘offset’ with a rotation along with a translation, and starts over a new ‘round’. The algorithm is shown as follows.

```
REPEAT if the target is not reached
	Rotate to the orientation facing the target
	Get the available distance forward by reading data from the sonar
	if available distance ≥ distance between the robot and the target
		Translate directly to the target
		Rotate to the targeted orientation
		BREAK
	else
		Translate by three quarters of the available distance
		Check whether the left-front or the right-front side is wider
		Left case: left rotate 60 degrees / right case: right rotate 60 degrees
		Get the available distance forward by reading data from the sonar
		Translate forward by half of the available distance
```

After a series of experiment, it is found that doing the ‘offset’ by always rotating the same degree sometimes causes the robot to get stuck. Thus, every 3 ‘round’, the displacement within the three ‘rounds’ is checked. If it is less than one-tenth of the distance to the target, the ‘offset’ for this run rotates by 120 degrees. This minor variation avoids the robot from the ‘stuck’ situation effectively.
