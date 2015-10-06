#pragma config(StandardModel, "EV3_REMBOT")
/////QUEUE//////
#define Head(Q) (Q).HEAD
#define Tail(Q) (Q).TAIL
#define InfoHead(Q) (Q).T[(Q).HEAD]
#define InfoTail(Q) (Q).T[(Q).TAIL]
#define maxEl(Q) (Q).MaxEl
#define Nil 0
/*** Definisi elemen dan address ***/
typedef int infotype;
typedef int address; /* indeks tabel */
/* *** Definisi Type Queue *** */
typedef struct {
	infotype T[10]; /* tabel penyimpan elemen */
	address HEAD; /*alamat penghapusan*/
	address TAIL; /*alamat penambahan*/
	int MaxEl; /*maksimum banyaknya elemen queue*/
} Queue;
int IsEmpty(Queue Q) {
	return(Head(Q)==Nil && Tail(Q)==Nil);
}
int IsFull(Queue Q) {
	int m = Head(Q) - Tail(Q);
	return((m==1) || (m==(-1*maxEl(Q)+1)));
}
int NBElmt (Queue Q)
{
	int n;
	if (IsEmpty(Q)) n = 0;
	else if (Head(Q) <= Tail(Q)) n = Tail(Q) - Head(Q) + 1;
	else n = Q.MaxEl - Tail(Q) + Head(Q) + 1;
	return n;
}
void CreateEmpty (Queue *Q)
{
	maxEl(*Q) = 10;
	Head(*Q) = Nil;
	Tail(*Q) = Nil;
}
void Add (Queue *Q,infotype X)
{
	if (IsEmpty(*Q))
	{
		Tail(*Q) = 1;
		Head(*Q) = 1;
	}
	else
	{
		Tail(*Q) += 1;
		if (Tail(*Q) == (*Q).MaxEl + 1) Tail(*Q) = 1;
	}
	InfoTail(*Q) = X;
}
void Del (Queue *Q,infotype *X)
{
	(*X) = InfoHead(*Q);
	if (Head(*Q) == Tail(*Q))	{
		Head(*Q) = Nil;
		Tail(*Q) = Nil;
	}
	else {
		Head(*Q)++;
		if (Head(*Q) == maxEl(*Q) + 1) Head(*Q) = 1;
	}
}
////////////////
#define red 1
#define green 0
int searchSpot() {
//menelusuri garis hitam sampai menemukan "color"
	int hue;
	int colortemp;
	int threshold = 65;
	resetGyro(gyroSensor);
	moveMotorTarget(leftMotor,360,100);
	moveMotorTarget(rightMotor,360,100);
	while(getMotorMoving(leftMotor)||getMotorMoving(rightMotor))
		sleep(1);
	while(1)
	{
		// sensor sees light:
		if(getColorReflected(colorSensor) < threshold)
		{
			// counter-steer right:
			motor[leftMotor]  = 55;
			motor[rightMotor] = 15;
		}
		// sensor sees dark:
		else
		{
			// counter-steer left:
			motor[leftMotor]  = 15;
			motor[rightMotor] = 55;
		}
		hue=getColorHue(colorSensor);
		if(hue==99 || hue==253) {
			colortemp=hue==99?green:red;
			break;
		}
	}
	return colortemp;
}

#define left 0
#define right 1
#define straight -1
#define first -2
void turn(int dir,int deg) {
//berbelok ke arah "dir", sebesar "deg" derajat
	int l = dir==left?0:30;
	int r = dir==right?0:30;
	resetGyro(gyroSensor);
	setMotorSpeed(leftMotor,l);
	setMotorSpeed(rightMotor,r);
	while(getMotorMoving(leftMotor) || getMotorMoving(rightMotor)) {
		sleep(1);
		if((getGyroDegrees(gyroSensor)<deg && dir==left)
			||(getGyroDegrees(gyroSensor)>deg && dir==right))
		break;
	}
}

int cekLine(int dir,int deg) {
//mengembalikan true jika menemukan garis
	int threshold = 65;
	int l = dir==left?0:30;
	int r = dir==right?0:30;
	resetGyro(gyroSensor);
	setMotorSpeed(leftMotor,l);
	setMotorSpeed(rightMotor,r);
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

void cekSimpang(Queue *Q) {
	if(cekLine(right,90)) {
		Add(Q,right);
		motor[leftMotor] = 0;
		motor[rightMotor] = 0;
	}
	if(cekLine(left,-90)) {
		Add(Q,straight);
		stepAhead(360);
	}
	if(cekLine(left,-90))
		Add(Q,left);
	turn(right,90);
}

void BFS(int dirBack,Queue *Q) {
	int x,y,degree,color;
	if(IsEmpty(*Q)) {}
	else {
		Del(Q,&x);
		switch(dirBack) {
			case right:
				y=(x==straight)?right:straight;
				break;
			case straight:
				y=(x==right)?left:right;
				break;
			case left:
				y=(x==straight)?left:straight;
				break;
			default:
				y=x; //first
		}
		if(y!=straight) {
			stepAhead(220);
			degree=(y==right)?85:-80;
			turn(y,degree);
		}
		color = searchSpot();
		if(color==green) {
			Add(Q,x);
			cekSimpang(Q);
		}
		turn(-1,-180);
		BFS(x,Q);
	}
}

task main()
{
	Queue Q;
	CreateEmpty(&Q);
	int color = searchSpot();
	if(color==green) {
		stepAhead(220);
		turn(right,20);
		cekSimpang(&Q);
	}
	BFS(first,&Q);
}
