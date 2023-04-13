//файл реализации Signal.h
#include "pch.h"
#include "Signl.h"

//Деструктор
Signal::~Signal() {
	X.~vector();
	Y.~vector();
	H.~vector();
	XR.~vector();
	keys.~vector();
	XN.~vector();
	YN.~vector();
	Err1.~vector();
	ErrK.~vector();
	Err2.~vector();
}

//заполнение сигнала
void Signal::FillSignal(double n, double Fd, double alfa, double max_noise, double A1, double A2, double A3,
	double G1, double G2, double G3, double niu1, double niu2, double niu3, Drawer* dr1, Drawer* dr2, Drawer* dr3, Drawer* dr4) {
	this->N = n;
	this->fd = Fd;
	this->alfa = alfa;
	this->A1 = A1;
	this->A2 = A2;
	this->A3 = A3;
	this->G1 = G1;
	this->G2 = G2;
	this->G3 = G3;
	this->niu1 = niu1;
	this->niu2 = niu2;
	this->niu3 = niu3;
	this->dr1 = dr1;
	this->dr2 = dr2;
	this->dr3 = dr3;
	this->dr4 = dr4;
	this->max_noise = max_noise;
	this->Create();
}
//отчистка сигнала
void Signal::ClearSignal() {
	X.clear();
	Y.clear();
	H.clear();
	XR.clear();
	keys.clear();
	XN.clear();
	YN.clear();
	Err1.clear();
	ErrK.clear();
	Err2.clear();
}

//создание сигнала
void Signal::Create() {
	srand(time(0));
	for (int i = 0; i < N; i++) {
		keys.push_back(i / fd);
		X.push_back(
			A1 * exp(-(keys[i] - niu1 / fd) * (keys[i] - niu1 / fd) / (G1 * G1)) +
			A2 * exp(-(keys[i] - niu2 / fd) * (keys[i] - niu2 / fd) / (G2 * G2)) +
			A3 * exp(-(keys[i] - niu3 / fd) * (keys[i] - niu3 / fd) / (G3 * G3))
		);

		if (i < N / 2) {
			if (i == 0)
				//H.push_back(0);
				//H.push_back(1);
				H.push_back(1);
			else
				H.push_back(abs((sin(alfa * keys[i]) / (alfa * keys[i]))));
		}

		else {
			H.push_back(H[N - 1 - i]);

		}
	}
}

//convolution
void Signal::Conv(vector<double> X, vector<double> H, vector<double>* Y) {
	for (int i = 0; i < N; i++) {
		double buf = 0;
		for (int k = 0; k < N; k++) {
			if (i - k < 0)
				buf += X[k] * H[k - i];
			else
				buf += X[k] * H[i - k];
		}
		Y->push_back(buf);
	}
}

//восстанавливает сигнал при разном уровне шума
void Signal::Solve(MSG msg) {
	noise = 0;
	double noise_step = max_noise / 100;

	//Наложение шума на исходный сигнал
	while (noise < max_noise) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		ErrK.push_back(noise);
		//Шум в исходном
		this->Noise(X, &XN);
		this->Conv(XN, H, &Y);
		this->Find(Y, H, &XR);
		Err1.push_back(FindError());
		//на гарфике с ошибкой
		dr3->DrawOne(Err1, 0, max_noise,
			*min_element(Err1.begin(), Err1.end()), *max_element(Err1.begin(), Err1.end()), 't', 'A', ErrK);
		dr1->DrawTwo(XR, X,
			*min_element(keys.begin(), keys.end()),
			*max_element(keys.begin(), keys.end()),
			min(*min_element(XR.begin(), XR.end()), *min_element(X.begin(), X.end())),
			max(*max_element(XR.begin(), XR.end()), *max_element(X.begin(), X.end())),
			't', 'A', keys, keys);
		XR.clear();
		XN.clear();
		Y.clear();
		XR.clear();

		noise += noise_step;
	}

	//наложение шума на сверку
	noise = 0;
	while (noise < max_noise) {
		ErrK.push_back(noise);

		this->Conv(X, H, &Y);
		this->Noise(Y, &YN);
		this->Find(YN, H, &XR);
		Err2.push_back(FindError());

		//на гарфике с ошибкой
		dr2->DrawTwo(Y, YN,
			*min_element(keys.begin(), keys.end()),
			*max_element(keys.begin(), keys.end()),
			min(*min_element(Y.begin(), Y.end()), *min_element(YN.begin(), YN.end())),
			max(*max_element(Y.begin(), Y.end()), *max_element(YN.begin(), YN.end())),
			't', 'A', keys, keys);
		dr4->DrawOne(Err2, 0, max_noise,
			*min_element(Err2.begin(), Err2.end()), *max_element(Err2.begin(), Err2.end()), 't', 'A', ErrK);
		dr1->DrawTwo(XR, X,
			*min_element(keys.begin(), keys.end()),
			*max_element(keys.begin(), keys.end()),
			min(*min_element(XR.begin(), XR.end()), *min_element(X.begin(), X.end())),
			max(*max_element(XR.begin(), XR.end()), *max_element(X.begin(), X.end())),
			't', 'A', keys, keys);
		XR.clear();
		YN.clear();
		Y.clear();
		XR.clear();

		noise += noise_step;
	}

	Err1.clear();
	Err2.clear();
	ErrK.clear();
}

//находит квадратичное отклонение
double Signal::FindError() {
	double buf = 0;
	for (int i = 0; i < N; i++)
		buf += (X[i] - XR[i]) * (X[i] - XR[i]);
	return buf;
}

//находит решение СЛАУ
void Signal::Find(vector <double> Y, vector<double> H, vector<double>* XR) {
	float
		* a = new float[N * N],
		* b = new float[N],
		* x = new float[N];

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i - j < 0)
				a[i * (int)N + j] = H[j - i];
			else
				a[i * (int)N + j] = H[i - j];
		}

	}
	for (int i = 0; i < N; i++)
		b[i] = Y[i];

	this->kazf(a, b, x);

	for (int i = 0; i < N; i++)
		XR->push_back(x[i]);

	delete a;
	delete b;
	delete x;
}

//метод Качмаржа
void Signal::kazf(float* a, float* b, float* x)
{
	int nn = N;
	int ny = N;
	// nn - количество неизвестных;  ny - количество уравнений
	float eps = 1.e-6f;
	int i, j, k;
	float s1, s2, fa1, t;
	float* x1;

	x1 = new float[nn];

	x[0] = 0.5f;
	for (i = 1; i < nn; i++)  x[i] = 0.f;

	s1 = s2 = 1.f;
	while (s1 > eps * s2)
	{
		for (i = 0; i < nn; i++) x1[i] = x[i];

		for (i = 0; i < ny; i++)
		{
			s1 = 0.0;
			s2 = 0.0;
			for (j = 0; j < nn; j++)
			{
				fa1 = a[i * nn + j];
				s1 += fa1 * x[j];
				s2 += fa1 * fa1;
			}
			t = (b[i] - s1) / s2;
			for (k = 0; k < nn; k++)    x[k] += a[i * nn + k] * t;
		}

		s1 = 0.0;
		s2 = 0.0;
		for (i = 0; i < nn; i++)
		{
			s1 += (x[i] - x1[i]) * (x[i] - x1[i]);
			s2 += x[i] * x[i];
		}
		s1 = (float)sqrt(s1);
		s2 = (float)sqrt(s2);
	}
	delete[] x1;
}

//функция рандома
double Signal::Random() {
	double a = 0;
	for (int i = 0; i < 12; i++)
		a += -1. + 2 * (double)rand() / RAND_MAX;
	return a;
}

//функция суммы квадратов
double Signal::Sum(vector<double> mas) {
	double a = 0;
	for (int i = 0; i < mas.size(); i++)
		a += mas[i] * mas[i];
	return a;
}

//функция зашумления
void Signal::Noise(vector<double> buf, vector<double>* res) {
	vector<double>
		n0, //вектор с некоррелированным шумом
		n;	//коррелированный шум
	double
		Es, //энергия сигнала
		En,	//энергия шума
		B,
		Al = noise / 100;	//коэффициенты пропорциональности

	//получаем некоррелированный белый шум
	for (int i = 0; i < N; i++)
		n0.push_back(Random());

	Es = this->Sum(buf);
	En = this->Sum(n0);

	B = sqrt(Al * Es / En);

	//получаем зашумленную шляпу
	for (int i = 0; i < N; i++) {
		res->push_back(buf[i] + B * n0[i]);
	}
	n0.~vector();
	n.~vector();
}

void Signal::ShowX() {
	noise = max_noise;

	//Шум в исходном
	this->Noise(X, &XN);
	this->Conv(XN, H, &Y);
	this->Find(Y, H, &XR);


	//Графики
	dr1->DrawTwo(XR, X,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		min(*min_element(XR.begin(), XR.end()), *min_element(X.begin(), X.end())),
		max(*max_element(XR.begin(), XR.end()), *max_element(X.begin(), X.end())),
		't', 'A', keys, keys);
	dr2->DrawOne(H,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		*min_element(H.begin(), H.end()),
		*max_element(H.begin(), H.end()), 't', 'A', ErrK);
	dr3->DrawOne(Y,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		*min_element(Y.begin(), Y.end()),
		*max_element(Y.begin(), Y.end()), 't', 'A', ErrK);
	dr4->DrawTwo(XN, X,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		min(*min_element(XN.begin(), XN.end()), *min_element(X.begin(), X.end())),
		max(*max_element(XN.begin(), XN.end()), *max_element(X.begin(), X.end())),
		't', 'A', keys, keys);

}

void Signal::ShowY() {
	noise = max_noise;

	//Шум в свертке
	this->Conv(X, H, &Y);
	this->Noise(Y, &YN);
	this->Find(YN, H, &XR);


	//Графики
	dr1->DrawTwo(XR, X,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		min(*min_element(XR.begin(), XR.end()), *min_element(X.begin(), X.end())),
		max(*max_element(XR.begin(), XR.end()), *max_element(X.begin(), X.end())),
		't', 'A', keys, keys);
	dr2->DrawOne(H,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		*min_element(H.begin(), H.end()),
		*max_element(H.begin(), H.end()), 't', 'A', ErrK);

	dr3->DrawTwo(Y, YN,
		*min_element(keys.begin(), keys.end()),
		*max_element(keys.begin(), keys.end()),
		min(*min_element(YN.begin(), YN.end()), *min_element(Y.begin(), Y.end())),
		max(*max_element(YN.begin(), YN.end()), *max_element(Y.begin(), Y.end())),
		't', 'A', keys, keys);
	dr4->DrawW();

}