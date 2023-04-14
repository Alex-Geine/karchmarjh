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

	//�������� �������
	void Create();

	//convolution
	void Conv(vector<double> X, vector<double> H, vector<double>* Y);
	
	//������� ������������ ����������
	double FindError();

	//������� ������� ����
	void Find(vector <double> Y, vector<double> H, vector<double>* XR);

	//����� ��������
	void kazf(float* a, float* b, float* x);

	//������� �������
	double Random();

	//������� ����� ���������
	double Sum(vector<double> mas);

	//������� ����������
	void Noise(vector<double> buf, vector<double>* res);

	
public:
	//���� �������� � ����� � X
	void ShowX();

	//���� �������� � ����� � Y
	void ShowY();

	//��������������� ������ ��� ������ ������ ����
	void Solve(MSG msg);

	//�������� �������
	void ClearSignal();

	//����������
	~Signal();

	//���������� �������
	void FillSignal(double n, double Fd, double alfa, double max_noise, double A1, double A2, double A3,
		double G1, double G2, double G3, double niu1, double niu2, double niu3, Drawer* dr1, Drawer* dr2, Drawer* dr3, Drawer* dr4);

	//�������� �� ������� �������
	bool IsClear();
};
