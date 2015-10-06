#pragma config(StandardModel, "EV3_REMBOT")
task main()
{
	int hue1, hue2;
	hue1 = getColorHue(colorSensor);
	motor[leftMotor]=100;
	motor[rightMotor]=100;
	while(1) {
		sleep(1);
		hue2 = getColorHue(colorSensor);
		if(hue1!=hue2) break;
	}
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}
