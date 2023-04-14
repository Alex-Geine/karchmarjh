#include <vector>
#include <algorithm>
#include <math.h>
#include "Drawer1.h"
#define Pi 3.1415926535
using namespace std;

class Signal {
private:
	Drawer
		*dr1,
		*dr2,
		*dr3,
		*dr4;
	double
		N,
		fd,
		alfa,		
		A1, 
		A2,
		A3,
		G1,
		G2,
		G3,
		niu1,
		niu2,
		niu3,
		noise,
		max_noise;
	vector<double>
		X,
		XN,
		XR,
		Y,
		H,
		YN,
		keys,
		ErrK,
		Err1,		
		Err2;

	//создание сигнала
	void Create();

	//convolution
	void Conv(vector<double> X, vector<double> H, vector<double>* Y);
	
	//находит квадратичное отклонение
	double FindError();

	//находит решение СЛАУ
	void Find(vector <double> Y, vector<double> H, vector<double>* XR);

	//метод Качмаржа
	void kazf(float* a, float* b, float* x);

	//функция рандома
	double Random();

	//функция суммы квадратов
	double Sum(vector<double> mas);

	//функция зашумления
	void Noise(vector<double> buf, vector<double>* res);

	
public:
	//одна итерация с шумом в X
	void ShowX();

	//одна итерация с шумом в Y
	void ShowY();

	//восстанавливает сигнал при разном уровне шума
	void Solve(MSG msg);

	//отчистка сигнала
	void ClearSignal();

	//Деструктор
	~Signal();

	//заполнение сигнала
	void FillSignal(double n, double Fd, double alfa, double max_noise, double A1, double A2, double A3,
		double G1, double G2, double G3, double niu1, double niu2, double niu3, Drawer* dr1, Drawer* dr2, Drawer* dr3, Drawer* dr4);

	//проверка на пустоту вектора
	bool IsClear();
};
