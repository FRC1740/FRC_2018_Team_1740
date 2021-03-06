#include "ThreeAxisDrive.h"

ThreeAxisDrive::ThreeAxisDrive()
{
	Requires(drivetrain);
//	Requires(camera);
}

void ThreeAxisDrive::Initialize()
{
	datalogger->Log("ThreeAxisDrive::Initialize()", STATUS_MESSAGE);
}

void ThreeAxisDrive::Execute()
{

	int inputPOV = 0;

	// The drivetrain->Go() method has built in handling for reversing motors on the left side...

	float x = 0, y = 0, t = 0; // floats for the axes x, y, twist
	float fl = 0, fr = 0, rl = 0, rr = 0; // floats for the motor outputs

	if (oi->threeAxisJoystick->GetY() > DEADBAND_YAXIS || oi->threeAxisJoystick->GetY() < -DEADBAND_YAXIS) // Deadband
	{
		y = SCALE_YAXIS * oi->threeAxisJoystick->GetY();
	}
	if (oi->threeAxisJoystick->GetX() > DEADBAND_XAXIS || oi->threeAxisJoystick->GetX() < -DEADBAND_XAXIS)  // Deadband
	{
		x = oi->threeAxisJoystick->GetX();
	}
	if (oi->threeAxisJoystick->GetTwist() > DEADBAND_TWIST || oi->threeAxisJoystick->GetTwist() < -DEADBAND_TWIST)  // Deadband
	{
		t = JOYSTICK_SCALE_TWIST  * oi->threeAxisJoystick->GetTwist();
	}
	fl = - y + t + x; // Front Left Wheel
	fr = - y - t - x; // Front Right Wheel
	rl = - y + t - x; // Rear Left Wheel
	rr = - y - t + x; // Rear Right Wheel

	// Only log the movement if something changed
	if (gfl!=fl || gfr!=fr || grl!=rl || grr!=rr)
	{
		gfl=fl; gfr=fr; grl=rl; grr=rr;
		char *data = new char[128];
		sprintf(data, "We're moving: %2.1f, %2.1f, %2.1f, %2.1f; X=%2.1f, Y=%2.1f, Twist=%2.1f", fl, fr, rl, rr, x, y, t);
		datalogger->Log(data, DEBUG_MESSAGE);
	}

	drivetrain->Go(fl,fr,rl,rr);
	/*
	// Alternatively, we can send the joystick inputs directly to the drivetrain->motor->Set() methods
	drivetrain->front_left_motor->Set(oi->threeAxisJoystick->GetTwist() - oi->threeAxisJoystick->GetY() - oi->threeAxisJoystick->GetX());
	drivetrain->front_right_motor->Set(oi->threeAxisJoystick->GetTwist() + oi->threeAxisJoystick->GetY() + oi->threeAxisJoystick->GetX());
	drivetrain->rear_left_motor->Set(oi->threeAxisJoystick->GetTwist() - oi->threeAxisJoystick->GetY() + oi->threeAxisJoystick->GetX());
	drivetrain->rear_right_motor->Set(oi->threeAxisJoystick->GetTwist() + oi->threeAxisJoystick->GetY() - oi->threeAxisJoystick->GetX());
	*/

	/*
	inputPOV = oi->threeAxisJoystick->GetPOV();

	if (inputPOV < -45 && inputPOV > -125) // Turn Camera Left
	{
			camera->TurnLeft();
	}
	else if (inputPOV > 45 && inputPOV < 125) // Turn Camera Right
	{
			camera->TurnRight();
	}
	*/
}

bool ThreeAxisDrive::IsFinished()
{
	return false;
}

void ThreeAxisDrive::End()
{
	drivetrain->Stop();
	datalogger->Log("ThreeAxisDrive::End()", ERROR_MESSAGE);
}

void ThreeAxisDrive::Interrupted()
{
	drivetrain->Stop();
	datalogger->Log("ThreeAxisDrive::Interrupted()", STATUS_MESSAGE);
}
