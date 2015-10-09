#pragma config(StandardModel, "EV3_REMBOT")
//Definitions
//#define 2t1 (float)100/255
//#define 1t2 (float)255/100

//Configuration
#define color_number 7 //Sesuaikan
#define color_tolerance 25

int color_lib[color_number][3]={
//  R   G   B  //Skala 0-100. kalau 0-255, tinggal kalikan 2t1. misal {  0,200*2t1,255*2t1}
	{100,100,100},	//0 Putih_Arena
	{  0,  0,  0},	//1 Hitam_Garis
	{  0,  0,100},	//2 Biru_Start
	{  0,100,  0},  //3 Ijo_Simpang
	{ 98, 31, 31},  //4 Merah_Ujung
	{100,  0,100},  //5 Pink_Buntu
	{  4, 72, 36}   //6 Ijo_Pudar
};

int check_color(){
	int res=-1; //if no color.
	int i=0,r,g,b;
	getColorRGB(colorSensor,r,g,b);//Baca warna
  displayTextLine(1,"R: %d, G: %d, B: %d",r,g,b);
	while(res==-1 && i<color_number){
		if(r>color_lib[i][0]-color_tolerance && r<color_lib[i][0]+color_tolerance &&
			 g>color_lib[i][1]-color_tolerance && g<color_lib[i][1]+color_tolerance &&
			 b>color_lib[i][2]-color_tolerance && b<color_lib[i][2]+color_tolerance){
			 res=i;
		}
		i++;
	}
	return res;
}

#define green 3
#define red 4
int searchSpot() {
//menelusuri garis hitam sampai menemukan "color"
	int colortemp;
	int threshold = 65;
	resetGyro(gyroSensor);
	moveMotorTarget(leftMotor,20,100);
	moveMotorTarget(rightMotor,20,100);
	while(getMotorMoving(leftMotor)||getMotorMoving(rightMotor))
		sleep(1);
	while(1)
	{
		// sensor sees light:
		if(getColorReflected(colorSensor) < threshold) {
			// counter-steer right:
			motor[leftMotor]  = 0;
			motor[rightMotor] = 0;
		}
		else {// sensor sees dark:
			// counter-steer left:
			motor[leftMotor]  = 0;
			motor[rightMotor] = 0;
		}
		colortemp=check_color()
		if(colortemp==3)
			break;
	}
	motor[leftMotor]=0;
	motor[rightMotor]=0;
	return colortemp;
}

#define left 0
#define right 1
void turn(int dir,int deg,int lmot,int rmot) {
//berbelok ke arah "dir", sebesar "deg" derajat
	resetGyro(gyroSensor);
	setMotorSpeed(leftMotor,lmot);
	setMotorSpeed(rightMotor,rmot);
	while(getMotorMoving(leftMotor) || getMotorMoving(rightMotor)) {
		sleep(1);
		if((getGyroDegrees(gyroSensor)<deg && dir==left)
			||(getGyroDegrees(gyroSensor)>deg && dir==right))
		break;
	}
}

int cekLine(int dir,int deg,int lmot,int rmot) {
//mengembalikan 1 jika menemukan garis
	int threshold = 65;
	resetGyro(gyroSensor);
	setMotorSpeed(leftMotor,lmot);
	setMotorSpeed(rightMotor,rmot);
	while(getMotorMoving(leftMotor) || getMotorMoving(rightMotor)) {
		sleep(1);
		if(getColorReflected(colorSensor) < threshold)
			return 1;
		else if((getGyroDegrees(gyroSensor)<deg && dir==left)
			||(getGyroDegrees(gyroSensor)>deg && dir==right))
			break;
	}
	return 0;
}

void stepAhead(int degree) {
//maju degree derajat putaran roda
	moveMotorTarget(leftMotor,degree,50);
	moveMotorTarget(rightMotor,degree,50);
	while(getMotorMoving(leftMotor)||getMotorMoving(rightMotor)) {
		sleep(1);
	}
}

void DFS() {
	int found = 0;
	int color = searchSpot();
	if(color==green) {
		sleep(100);
		stepAhead(20);
		turn(right,20,30,-30);
		if(cekLine(right,70,30,-30)==1)
			turn(right,30,30,-30);
		else if(cekLine(left,-90,-30,30))
			turn(right,20,30,-30);
		else if(cekLine(left,-90,-40,30));
	}
	else if(color==red) {
		turn(left,-180,-40,30);
	}
	DFS();
}

task main() {
	DFS();
}
