#pragma once

#include <afxwin.h>
#include <vector>
//#include <amp_graphics.h>
//#define _SILENCE_AMP_DEPRECATION_WARNINGS
using namespace std;

class Drawer
{
	// ������������� ������� ���������.
	CRect frame;
	// ��������� �� ������ ����, ������� ���������.
	CWnd * wnd;
	// �������� ���������, ����������� � ������� ���������.
	CDC * dc;
	// �������� ���������, ����������� � ������.
	CDC memDC;
	// ������ ��� ��������� ��������� memDC.
	CBitmap bmp;
	// ���� ��� ������������ ��������� ������������� ������.
	bool init;
public:
	// ������������������� ������ ������ �� ������ HWND.
	void Create(HWND hWnd)
	{
		// �������� ��������� �� ����.
		wnd = CWnd::FromHandle(hWnd);
		// �������� ������������� ����.
		wnd->GetClientRect(frame);
		// �������� �������� ��� ��������� � ���� ����.
		dc = wnd->GetDC();

		// ������� �����-��������.
		memDC.CreateCompatibleDC(dc);
		// ������� ����� ��� ��������� ���������.
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		// �������� ����� ��� ������������� �����-����������.
		memDC.SelectObject(&bmp);
		init = true;
	}

	void DrawFon(double data_x_min, double data_x_max, double data_y_min, double data_y_max)
	{

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// ������ ����� � ��������.
		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	
	// ���������� ������ �� ���������� ������.
	void DrawThree(vector<double> &data, vector<double>& data1, vector<double>& data2, double data_x_min, double data_x_max, double data_y_min, double data_y_max,
		
		vector<double>& keys = vector<double>()
	)
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_penq(PS_SOLID, 2, RGB(0, 0, 255));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(15, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;
		
	
		// ������ ����� � ��������.
		unsigned int grid_size = 10;

	
	
		// ������ ������.
		if (data.empty()) return;

		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}

		memDC.SelectObject(&data_pen);

		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);
		vector<double> y1 = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> y2 = convert_range(data2, actual_top, actual_bottom, data_y_max, data_y_min);

		
				
		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}
		memDC.SelectObject(&data_penq);
		memDC.MoveTo(x[0], y1[0]);
		for (unsigned int i = 0; i < y1.size(); i++)
		{
			memDC.LineTo(x[i], y1[i]);
		}
		memDC.MoveTo(x[0], y2[0]);
		for (unsigned int i = 0; i < y2.size(); i++)
		{
			memDC.LineTo(x[i], y2[i]);
		}



		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size/2 + 1; i++)
		{
			CString str;
			str.Format(L"%.3f", data_x_min + i*(data_x_max - data_x_min) /(grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding/2, str);

			str.Format(L"%.3f", data_y_min + i*(data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5*left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}
		
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	void ReDraw() {
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	// ���������� ������ �� ���������� ������.
	void Draw(vector<double>& data, double data_x_min, double data_x_max, double data_y_min, double data_y_max, vector<double>& keys = vector<double>())
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// ������ ����� � ��������.
		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		// ������ ������.
		if (data.empty()) return;

		

		memDC.SelectObject(&data_pen);

		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_top, actual_bottom, data_x_max, data_x_min);
	
		
		
		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{		
			memDC.LineTo(x[i], y[i]);
		}

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	vector<double> convert_range(vector <double> & data, double outmax, double outmin, double inmax, double inmin)
	{
		vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto & item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
	double convert(double data, double outmax, double outmin, double inmax, double inmin)
	{
		double output = data;
		double k = (outmax - outmin) / (inmax - inmin);

		output = (output - inmin) * k + outmin;


		return output;

	}
	//������ ��� ���������� ��������� �������� �� ����������
	double xr(double r, double t, double w) {
		return r * cos(w * t);
	}
	double yr(double r, double t, double w) {
		return r * sin(w * t);
	}
	
	// ���������� ������ �� ���������� ������.
	void DrawW()
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// ������ ����� � ��������.
		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

	



		
		


		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	// ���������� ������ �� ���������� ������.
	void DrawOne(vector<double>& data, double data_x_min, double data_x_max, double data_y_min, double data_y_max, char A1, char A2,vector<double>& keys = vector<double>())
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(15, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);
	
		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// ������ ����� � ��������.
		unsigned int grid_size = 10;
		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));
		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		// ������ ������.
		if (data.empty()) return;

		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}

		memDC.SelectObject(&data_pen);

		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		
		//������� � ��������
		CString a1, a2;
		a1 = A1;
		a2 = A2;
		memDC.TextOutW(45, 0, a1);
		memDC.TextOutW(670, 80, a2);

		//��� ����� ������� ����������� �������
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.3f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.3f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void DrawTwo(vector<double>& data,vector<double>& data1, double data_x_min, double data_x_max, double data_y_min, double data_y_max,char A1,char A2, vector<double>& keys = vector<double>(), vector<double>& keys1 = vector<double>())
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data1_pen(PS_SOLID, 2, RGB(0, 0, 250));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(15, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// ������ ����� � ��������.
		unsigned int grid_size = 10;
		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));
		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		// ������ ������.
		if (data.empty()) return;

		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}

		memDC.SelectObject(&data_pen);

		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);
		vector<double> y1 = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x1 = convert_range(keys1, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}
		memDC.SelectObject(&data1_pen);
		memDC.MoveTo(x1[0], y1[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x1[i], y1[i]);
		}

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));

		//������� � ��������
		CString a1, a2;
		a1 = A1;
		a2 = A2;
		memDC.TextOutW(45, 0, a1);
		memDC.TextOutW(670, 80, a2);
		//��� ����� ������� ����������� �������
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.3f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.3f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	void DrawTwo(vector<double>& data, vector<double>& data1, double data_x_min, double data_x_max, double data_y_min, double data_y_max, char A1, char A2,  bool flag, vector<double>& keys = vector<double>(), vector<double>& keys1 = vector<double>())
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data1_pen(PS_SOLID, 2, RGB(0, 0, 250));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(15, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// ������ ����� � ��������.
		unsigned int grid_size = 10;
		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));
		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		// ������ ������.
		if (data.empty()) return;

		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}

		memDC.SelectObject(&data_pen);

		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);
		vector<double> y1 = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x1 = convert_range(keys1, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}
		memDC.SelectObject(&data1_pen);
		memDC.MoveTo(x1[0], y1[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x1[i], y1[i]);
		}

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));

		//������� � ��������
		CString a1, a2;
		a1 = A1;
		a2 = A2;
		memDC.TextOutW(45, 0, a1);
		memDC.TextOutW(670, 80, a2);
		//��� ����� ������� ����������� �������
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.3f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.3f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		//dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	void DrawMain(vector<double>& data,  double data_x_min, double data_x_max, double data_y_min, double data_y_max,   vector<double >t
		//, double x0,
		//double y0, 
		//vector<double>& keys = vector<double>()
	)
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 2, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_penq(PS_SOLID, 2, RGB(255, 220, 20));
		CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// ������
			FALSE,				// ������������
			FALSE,				// �������������
			DEFAULT_CHARSET,	// ����� ��������
			OUT_OUTLINE_PRECIS,	// �������� ������������.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// ��������
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;


		// ������ ����� � ��������.
		unsigned int grid_size = 10;

		// ����� ���.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));


		memDC.SelectObject(&data_pen);

		vector<double> x = convert_range(data, frame.Width(), 0, data_x_max, data_x_min);
	
		
		
		
		//	����� L/2
		memDC.MoveTo(frame.Width() / 2, frame.Height());
		memDC.LineTo(frame.Width() / 2, 0);
		// ����� ������ ���������
		
		double half = (frame.Width()) / 2;// -40;
		double L_left = (frame.Width()) / 3 - (frame.Width()) / 6;
		double L_right = (frame.Width()) * 2 / 3 + (frame.Width()) / 6;
		double x0 = half + x[0];
		//��������� ������
		memDC.SelectObject(&grid_pen);
		memDC.Ellipse(frame.Width()  / 3 - 40, 213, frame.Width()  / 3 + 40, 290);
		memDC.Ellipse(frame.Width() *2/ 3 - 40, 213, frame.Width()*2  / 3 + 40, 290);
		
		memDC.MoveTo(frame.Width() / 3, 251.5);
		memDC.LineTo(frame.Width() / 3, frame.Height());
		memDC.MoveTo(frame.Width() *2/ 3, 251.5);
		memDC.LineTo(frame.Width() *2/ 3, frame.Height());

		//��������� ����������� �����
		double w = 4,
			
		//����� �����
			r1 = frame.Width() / 3 ;

		double rx1 = 20 * cos(w * t[0]) + r1;
		double ry1 = 20 * sin(w * t[0]) + 251.5;
		memDC.SelectObject(&grid_pen);
		memDC.Ellipse(rx1 - 5, ry1 - 5, rx1 + 5, ry1 +5);
		
		//������ �����
		double r2 = frame.Width() *2/ 3;

		double rx2 = -20 * cos(w * t[0]) + r2;
		double ry2 = 20 * sin(w * t[0]) + 251.5;
		memDC.SelectObject(&grid_pen);
		memDC.Ellipse(rx2 - 5, ry2 - 5, rx2 + 5, ry2 + 5);
		
		
		double y0 = 200;
		//������ ����� �����
		memDC.MoveTo(L_left + x[0] , y0 + 12);
		memDC.LineTo(L_right + x[0], y0 + 12);
		//������ ���
		memDC.LineTo(L_right + x[0], y0 - 12);
		//������� �����
		memDC.LineTo(L_left + x[0], y0 - 12);
		//����� ���
		memDC.LineTo(L_left + x[0], y0 + 12);

		memDC.Ellipse( x0- 10, y0 - 10, x0 + 10, y0 + 10);
		
		

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	};