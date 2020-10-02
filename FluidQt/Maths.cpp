#include "Maths.h"

matrix::matrix()
{
	rows = 4;
	columns = 4;
	data = new float* [4];
	for (int i = 0; i < 4; i++) {
		data[i] = new float[4];
		for (int j = 0; j < 4; j++) {
			data[i][j] = 0.0;
		}
	}
}
matrix::matrix(const matrix& m)
{
	rows = m.rows;
	columns = m.columns;
	data = new float* [rows];
	for (int i = 0; i < rows; i++) {
		data[i] = new float[columns];
		for (int j = 0; j < columns; j++) {
			data[i][j] = m[i][j];
		}
	}
}
matrix& matrix::operator=(const matrix& m)
{
	if (this == &m) {
		return *this;
	}
	for (int i = 0; i < rows; i++) {
		delete[] data[i];
	}
	delete[] data;

	rows = m.rows;
	columns = m.columns;
	data = new float* [rows];
	for (int i = 0; i < rows; i++) {
		data[i] = new float[columns];
		for (int j = 0; j < columns; j++) {
			data[i][j] = m[i][j];
		}
	}
}
void matrix::identity()
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (i == j)data[i][j] = 1.0;
			else data[i][j] = 0.0;
		}
	}
}
float*& matrix::operator[](int e) const
{
	return data[e];
}
matrix::matrix(int r, int c)
{
	rows = r;
	columns = c;
	data = new float* [r];
	for (int i = 0; i < r; i++) {
		data[i] = new float[c];
		for (int j = 0; j < c; j++) {
			data[i][j] = 0.0;
		}
	}
}
std::string matrix::toString()
{
	std::string res;
	res += "[";
	for (int i = 0; i < rows; i++) {
		res += "[";
		for (int j = 0; j < columns; j++) {
			res += std::to_string(data[i][j]);
			if (j != columns - 1)res += ",";
		}
		res += "]\n";
	}
	res += "]";
	return res;
}
matrix::~matrix()
{
	for (int i = 0; i < rows; i++) {
		delete[] data[i];
	}
	delete[] data;
}
matrix operator*(const matrix& n, const matrix& m)
{
	matrix res(n.rows, m.columns);

	for (int i = 0; i < n.rows; i++) {
		for (int j = 0; j < m.columns; j++) {
			for (int k = 0; k < n.columns; k++) {
				res.data[i][j] += n.data[i][k] * m.data[k][j];
			}
		}
	}

	return res;
}
matrix operator*(const matrix& n, const double& d)
{
	matrix res = n;

	for (int i = 0; i < n.rows; i++) {
		for (int j = 0; j < n.columns; j++) {
			res[i][j] *= d;
		}
	}
	return res;
}
vector4::vector4()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
}
vector4::vector4(float d)
{
	x = d;
	y = d;
	z = d;
	w = d;
}
vector4::~vector4() {}
vector4::vector4(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}
vector2::vector2()
{
	x = 0.0;
	y = 0.0;
}
vector2::vector2(float d)
{
	x = d;
	y = d;
}
vector2::~vector2() {}
vector2::vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}
vector3 maths::cross(const vector3& a, const vector3& b)
{
	vector3 result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;

	return result;
}
__host__ __device__
vector3 maths::normalize(const vector3& v)
{
	vector3 result = v;
	double l = length(v);
	result.x /= l;
	result.y /= l;
	result.z /= l;

	return result;
}
__host__ __device__
float maths::length(const vector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
__host__ __device__
double maths::dot(const vector3& a, const vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
matrix maths::translate(const matrix& mat, const vector3& v)
{
	matrix res = mat;
	res[0][3] = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3];
	res[1][3] = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3];
	res[2][3] = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3];
	res[3][3] = mat[3][0] * v.x + mat[3][1] * v.y + mat[3][2] * v.z + mat[3][3];

	return res;
}
matrix maths::scale(const matrix& mat, const vector3& v)
{
	matrix res = mat;
	res[0][0] = res[0][0] * v.x;
	res[0][1] = res[0][1] * v.x;
	res[0][2] = res[0][2] * v.x;

	res[1][0] = res[1][0] * v.y;
	res[1][1] = res[1][1] * v.y;
	res[1][2] = res[1][2] * v.y;

	res[2][0] = res[2][0] * v.z;
	res[2][1] = res[2][1] * v.z;
	res[2][2] = res[2][2] * v.z;

	return res;
}
matrix maths::rotate(matrix mat, double angle, const vector3& v)
{
	matrix result(4, 4);
	result.identity();
	double a = angle;
	double c = cos(a);
	double s = sin(a);

	vector3 axis = normalize(v);
	vector3  temp = axis * (1.0 - c);
	result[0][0] = c + temp.x * axis.x;
	result[0][1] = temp.x * axis.y + s * axis.z;
	result[0][2] = temp.x * axis.z - s * axis.y;

	result[1][0] = temp.y * axis.x - s * axis.z;
	result[1][1] = c + temp.y * axis.y;
	result[1][2] = temp.y * axis.z + s * axis.x;

	result[2][0] = temp.z * axis.x + s * axis.y;
	result[2][1] = temp.z * axis.y - s * axis.x;
	result[2][2] = c + temp.z * axis.z;

	matrix R = mat;
	R[0][0] = mat[0][0] * result[0][0] + mat[1][0] * result[0][1] + mat[2][0] * result[0][2];
	R[0][1] = mat[0][1] * result[0][0] + mat[1][1] * result[0][1] + mat[2][1] * result[0][2];
	R[0][2] = mat[0][2] * result[0][0] + mat[1][2] * result[0][1] + mat[2][2] * result[0][2];

	R[1][0] = mat[0][0] * result[1][0] + mat[1][0] * result[1][1] + mat[2][0] * result[1][2];
	R[1][1] = mat[0][1] * result[1][0] + mat[1][1] * result[1][1] + mat[2][1] * result[1][2];
	R[1][2] = mat[0][2] * result[1][0] + mat[1][2] * result[1][1] + mat[2][2] * result[1][2];

	R[2][0] = mat[0][0] * result[2][0] + mat[1][0] * result[2][1] + mat[2][0] * result[2][2];
	R[2][1] = mat[0][1] * result[2][0] + mat[1][1] * result[2][1] + mat[2][1] * result[2][2];
	R[2][2] = mat[0][2] * result[2][0] + mat[1][2] * result[2][1] + mat[2][2] * result[2][2];

	R[3][0] = mat[3][0];
	R[3][1] = mat[3][1];
	R[3][2] = mat[3][2];


	return R;
}
mat4x4 maths::createTransformationMatrix(const vector3& pos, const vector3& rot, const vector3& dim)
{
	mat4x4 res;
	res.identity();
	res = translate(res, pos);

	res = rotate(res, rot.x, vector3(1, 0, 0));
	res = rotate(res, rot.y, vector3(0, 1, 0));
	res = rotate(res, rot.z, vector3(0, 0, 1));
	res = scale(res, dim);

	return res;
}
vector3 operator*(const vector3& v, const double& d)
{
	vector3 res = v;
	res.x *= d;
	res.y *= d;
	res.z *= d;

	return res;
}
vector4 operator*(const matrix& m, const vector4& v)
{
	vector4 result;

	result.x = m[0][0] * v.x + m[0][1] * v.x + m[0][2] * v.x + m[0][3] * v.x;
	result.y = m[1][0] * v.y + m[1][1] * v.y + m[1][2] * v.y + m[1][3] * v.y;
	result.z = m[2][0] * v.z + m[2][1] * v.z + m[2][2] * v.z + m[2][3] * v.z;
	result.w = m[3][0] * v.w + m[3][1] * v.w + m[3][2] * v.w + m[3][3] * v.w;

	return result;
}
double matrix::determinant2x2()
{
	if (rows == 2 && columns == 2) {
		return data[0][0] * data[1][1] - data[0][1] * data[1][0];
	}
}
double matrix::determinant(const matrix& mat)
{
	int i, j, j1, j2, n = mat.columns;
	double det = 0;
	matrix res;

	if (n == 2) {
		det = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
	}
	else {
		det = 0;
		for (j1 = 0; j1 < n; j1++) {
			res = matrix(mat.rows - 1, mat.columns - 1);
			for (i = 1; i < n; i++) {
				j2 = 0;
				for (j = 0; j < n; j++) {
					if (j == j1)
						continue;
					res[i - 1][j2] = mat[i][j];
					j2++;
				}
			}
			det += pow(-1.0, 1.0 + j1 + 1.0) * mat[0][j1] * determinant(res);
		}
	}
	return(det);
}
matrix matrix::getCoFactor(const matrix& mat)
{
	int i, j, ii, jj, i1, j1;
	double det;

	matrix temp(mat.rows - 1, mat.columns - 1);
	matrix res = mat;
	for (j = 0; j < mat.rows; j++) {
		for (i = 0; i < mat.columns; i++) {
			i1 = 0;
			for (ii = 0; ii < mat.rows; ii++) {
				if (ii == i)
					continue;
				j1 = 0;
				for (jj = 0; jj < mat.rows; jj++) {
					if (jj == j)
						continue;
					temp[i1][j1] = mat[ii][jj];
					j1++;
				}
				i1++;
			}
			det = determinant(temp);
			res[i][j] = pow(-1.0, i + j + 2.0) * det;
		}
	}
	return res;
}
matrix maths::getCoFactor(const matrix& mat)
{
	int i, j, ii, jj, i1, j1;
	double det;

	matrix temp(mat.rows - 1, mat.columns - 1);
	matrix res = mat;
	for (j = 0; j < mat.rows; j++) {
		for (i = 0; i < mat.columns; i++) {
			i1 = 0;
			for (ii = 0; ii < mat.rows; ii++) {
				if (ii == i)
					continue;
				j1 = 0;
				for (jj = 0; jj < mat.rows; jj++) {
					if (jj == j)
						continue;
					temp[i1][j1] = mat[ii][jj];
					j1++;
				}
				i1++;
			}
			det = determinant(temp);
			res[i][j] = pow(-1.0, i + j + 2.0) * det;
		}
	}
	return res;
}
double maths::determinant(const matrix& mat)
{
	int i, j, j1, j2, n = mat.columns;
	double det = 0;
	matrix res;

	if (n == 2) {
		det = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
	}
	else {
		det = 0;
		for (j1 = 0; j1 < n; j1++) {
			res = matrix(mat.rows - 1, mat.columns - 1);
			for (i = 1; i < n; i++) {
				j2 = 0;
				for (j = 0; j < n; j++) {
					if (j == j1)
						continue;
					res[i - 1][j2] = mat[i][j];
					j2++;
				}
			}
			det += pow(-1.0, 1.0 + j1 + 1.0) * mat[0][j1] * determinant(res);
		}
	}
	return(det);
}
matrix maths::invert(const matrix& mat)
{
	matrix res = getCoFactor(mat);
	double det = determinant(mat);
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.columns; j++) {
			res[i][j] /= det;
		}
	}
	return res;
}
/*matrix maths::lookAtP(vector3 eye, vector3 center, vector3 up)
{
	vector3 forward(0, 0, 0);
	vector3 upt;

	forward = center - eye;
	forward = normalize(forward);

	vector3 side(0, 0, 0);
	side = cross(forward, up);
	side = normalize(side);


	upt = cross(side, forward);


	matrix m;
	m.identity();
	m[0][0] = side.x;
	m[0][1] = side.y;
	m[0][2] = side.z;
	m[1][0] = upt.x;
	m[1][1] = upt.y;
	m[1][2] = upt.z;
	m[2][0] = -forward.x;
	m[2][1] = -forward.y;
	m[2][2] = -forward.z;

	m = invert(m);


	return m;
}*/
vector3 operator+(const vector3& d, const vector3& v) {
	vector3 res;
	res.x = d.x + v.x;
	res.y = d.y + v.y;
	res.z = d.z + v.z;

	return res;
}
mat4x4 maths::createViewMatrix(Camera camera)
{
	mat4x4 view;
	view.identity();
	view = lookAtP(camera.position, camera.pointToLookAt, vector3(0, 1, 0));
	view = translate(view, -camera.position);

	return view;
}
mat4x4 maths::createProjectionMatrix(float FOV, float width, float height, float NEAR_PLANE, float FAR_PLANE)
{
	/*float aspectRatio = (float)State::WIDTH / (float)State::HEIGHT;
	float y_scale = (float)((1.0f / tan(((FOV / 2.0f) / 360)*3.14f)) * aspectRatio);
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;

	mat4x4 projectionMatrix = mat4x4();
	projectionMatrix[0][0] = x_scale;
	projectionMatrix[1][1] = y_scale;
	projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix[2][3] = -1;
	projectionMatrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix[3][3] = 0;

	return projectionMatrix;*/

	float aspectRatio = (float)width / (float)height;
	float y_scale = (float)((1.0f / tan(((FOV / 2.0f) / 360) * 3.14f)) * aspectRatio);
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;

	mat4x4 projectionMatrix;
	projectionMatrix.identity();
	projectionMatrix[0] = x_scale;
	projectionMatrix[5] = y_scale;
	projectionMatrix[10] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix[11] = -1.0f;
	projectionMatrix[14] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix[15] = 0.0f;

	return projectionMatrix;
}
vector3 operator-(const vector3& v)
{
	vector3 res;
	res.x = -v.x;
	res.y = -v.y;
	res.z = -v.z;

	return res;
}
mat4x4 maths::ortho(const double& left, const double& right, const double& bottom, const double& top,
	const double& zNear, const double& zFar)
{
	mat4x4 res;
	res.identity();
	res[0] = 2.0 / (right - left);
	res[5] = 2.0 / (top - bottom);
	res[10] = -2 / (zFar - zNear);
	res[12] = -((right + left) / (right - left));
	res[13] = -((top + bottom) / (top - bottom));
	res[14] = -((zFar + zNear) / (zFar - zNear));

	return res;
}
void operator*=(vector3& v, const double& d)
{
	v.x *= d;
	v.y *= d;
	v.z *= d;
}
vector3 operator*(const vector3& v, const vector3& d)
{
	vector3 res;
	res.x = v.x * d.x;
	res.y = v.y * d.y;
	res.z = v.z * d.z;

	return res;
}
void operator*=(vector3& v, const vector3& d)
{
	v.x *= d.x;
	v.y *= d.y;
	v.z *= d.z;
}
void operator+=(vector3& v, const vector3& d)
{
	v.x += d.x;
	v.y += d.y;
	v.z += d.z;
}
void operator-=(vector3& v, const vector3& d)
{
	v.x -= d.x;
	v.y -= d.y;
	v.z -= d.z;
}
vector3 operator+(const vector3& v, const float& d)
{
	vector3 res;
	res.x = v.x + d;
	res.y = v.y + d;
	res.z = v.z + d;

	return res;
}
vector4 operator+(const vector4& v, const vector4& d)
{
	vector4 res;
	res.x = v.x + d.x;
	res.y = v.y + d.y;
	res.z = v.z + d.z;
	res.w = v.w + d.w;

	return res;
}
void operator+=(vector4& v, const vector4& d)
{
	v.x += d.x;
	v.y += d.y;
	v.z += d.z;
	v.w += d.w;
}
mat4x4::mat4x4()
{
	data = new float[16];
	for (int i = 0; i < 16; i++) {
		data[i] = 0.0f;
	}
}
mat4x4::~mat4x4()
{
	delete[] data;
}
mat4x4 operator*(const mat4x4& m, const mat4x4& n)
{
	mat4x4 res;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				res.data[i * 4 + j] += n.data[i * 4 + k] * m.data[k * 4 + j];
			}
		}
	}

	return res;
}
mat4x4::mat4x4(const mat4x4& m)
{
	data = new float[16];
	for (int i = 0; i < 16; i++) {
		data[i] = m.data[i];
	}
}
float& mat4x4::operator[](int e) const
{
	return data[e];
}
mat4x4& mat4x4::operator=(const mat4x4& m)
{
	if (this == &m) {
		return *this;
	}
	delete[] data;

	data = new float[16];
	for (int i = 0; i < 16; i++) {
		data[i] = m.data[i];
	}
}
mat4x4 maths::invert(const mat4x4& m)
{
	mat4x4 result;
	float inv[16], det;
	int i;
	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		result[i] = inv[i] * det;

	return result;
}
mat4x4 maths::translate(const mat4x4& mat, const vector3& v)
{
	mat4x4 res = mat;
	res[12] = mat[0] * v.x + mat[4] * v.y + mat[8] * v.z + mat[12];
	res[13] = mat[1] * v.x + mat[5] * v.y + mat[9] * v.z + mat[13];
	res[14] = mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14];
	res[15] = mat[3] * v.x + mat[7] * v.y + mat[11] * v.z + mat[15];

	/*mat4x4 res = mat;
	res[3] = mat[0] * v.x + mat[1] * v.y + mat[2] * v.z + mat[3];
	res[7] = mat[4] * v.x + mat[5] * v.y + mat[6] * v.z + mat[7];
	res[11] = mat[8] * v.x + mat[9] * v.y + mat[10] * v.z + mat[11];
	res[15] = mat[12] * v.x + mat[13] * v.y + mat[14] * v.z + mat[15];*/

	return res;
}
void mat4x4::identity()
{
	data[0] = 1.0;
	data[5] = 1.0;
	data[10] = 1.0;
	data[15] = 1.0;
}
mat4x4 maths::rotate(const mat4x4& mat, const float& angle, const vector3& v)
{
	/*mat4x4 result;
	result.identity();
	double a = angle;
	double c = cos(a);
	double s = sin(a);

	vector3 axis = normalize(v);
	vector3  temp = axis * (1.0 - c);
	result[1] = c + temp.x * axis.x;
	result[4] = temp.x * axis.y + s * axis.z;
	result[8] = temp.x * axis.z - s * axis.y;

	result[1] = temp.y * axis.x - s * axis.z;
	result[5] = c + temp.y * axis.y;
	result[9] = temp.y * axis.z + s * axis.x;

	result[2] = temp.z * axis.x + s * axis.y;
	result[6] = temp.z * axis.y - s * axis.x;
	result[10] = c + temp.z * axis.z;

	mat4x4 R = mat;
	R[0] = mat[0] * result[0] + mat[1] * result[4] + mat[2] * result[8];
	R[4] = mat[4] * result[0] + mat[5] * result[4] + mat[6] * result[8];
	R[8] = mat[8] * result[0] + mat[9] * result[4] + mat[10] * result[8];

	R[1] = mat[0] * result[1] + mat[1] * result[5] + mat[2] * result[9];
	R[5] = mat[4] * result[1] + mat[5] * result[5] + mat[6] * result[9];
	R[9] = mat[8] * result[1] + mat[9] * result[5] + mat[10] * result[9];

	R[2] = mat[0] * result[2] + mat[1] * result[6] + mat[2] * result[10];
	R[6] = mat[4] * result[2] + mat[5] * result[6] + mat[6] * result[10];
	R[10] = mat[8] * result[2] + mat[9] * result[6] + mat[10] * result[10];

	R[3] = mat[3];
	R[7] = mat[7];
	R[11] = mat[11];


	return R;*/
	mat4x4 result;
	result.identity();
	double a = angle;
	double c = cos(a);
	double s = sin(a);

	vector3 axis = normalize(v);
	vector3  temp = axis * (1.0 - c);
	result[0] = c + temp.x * axis.x;
	result[1] = temp.x * axis.y + s * axis.z;
	result[2] = temp.x * axis.z - s * axis.y;

	result[4] = temp.y * axis.x - s * axis.z;
	result[5] = c + temp.y * axis.y;
	result[6] = temp.y * axis.z + s * axis.x;

	result[8] = temp.z * axis.x + s * axis.y;
	result[9] = temp.z * axis.y - s * axis.x;
	result[10] = c + temp.z * axis.z;

	mat4x4 R = mat;
	R[0] = mat[0] * result[0] + mat[4] * result[1] + mat[8] * result[2];
	R[1] = mat[1] * result[0] + mat[5] * result[1] + mat[9] * result[2];
	R[2] = mat[2] * result[0] + mat[6] * result[1] + mat[10] * result[2];

	R[4] = mat[0] * result[4] + mat[4] * result[5] + mat[8] * result[6];
	R[5] = mat[1] * result[4] + mat[5] * result[5] + mat[9] * result[6];
	R[6] = mat[2] * result[4] + mat[6] * result[5] + mat[10] * result[6];

	R[8] = mat[0] * result[8] + mat[4] * result[9] + mat[8] * result[10];
	R[9] = mat[1] * result[8] + mat[5] * result[9] + mat[9] * result[10];
	R[10] = mat[2] * result[8] + mat[6] * result[9] + mat[10] * result[10];

	R[12] = mat[12];
	R[13] = mat[13];
	R[14] = mat[14];


	return R;
}
mat4x4 maths::scale(const mat4x4& mat, const vector3& v)
{
	mat4x4 res = mat;
	res[0] = res[0] * v.x;
	res[1] = res[1] * v.x;
	res[2] = res[2] * v.x;

	res[4] = res[4] * v.y;
	res[5] = res[5] * v.y;
	res[6] = res[6] * v.y;

	res[8] = res[8] * v.z;
	res[9] = res[9] * v.z;
	res[10] = res[10] * v.z;

	return res;

	/*mat4x4 res = mat;
	res[0] = res[0] * v.x;
	res[4] = res[4] * v.x;
	res[8] = res[8] * v.x;

	res[1] = res[1] * v.y;
	res[5] = res[5] * v.y;
	res[9] = res[9] * v.y;

	res[2] = res[2] * v.z;
	res[6] = res[6] * v.z;
	res[10] = res[10] * v.z;

	return res;*/
}
mat4x4 maths::lookAtP(const vector3& eye, const vector3& center, const vector3& up)
{
	vector3 forward(0, 0, 0);
	vector3 upt;

	forward = center - eye;
	forward = normalize(forward);

	vector3 side(0, 0, 0);
	side = cross(forward, up);
	side = normalize(side);


	upt = cross(side, forward);


	mat4x4 m;
	m.identity();
	m[0] = side.x;
	m[1] = side.y;
	m[2] = side.z;
	m[4] = upt.x;
	m[5] = upt.y;
	m[6] = upt.z;
	m[8] = -forward.x;
	m[9] = -forward.y;
	m[10] = -forward.z;


	m = invert(m);


	return m;
}
vector4 operator*(const mat4x4& m, const vector4& v)
{
	vector4 result;

	result.x = m[0] * v.x + m[1] * v.x + m[2] * v.x + m[3] * v.x;
	result.y = m[4] * v.y + m[5] * v.y + m[6] * v.y + m[7] * v.y;
	result.z = m[8] * v.z + m[9] * v.z + m[10] * v.z + m[11] * v.z;
	result.w = m[12] * v.w + m[13] * v.w + m[14] * v.w + m[15] * v.w;

	return result;
}
vector2 operator+(const vector2& v, const vector2& u)
{
	vector2 result;

	result.x = v.x + u.x;
	result.y = v.x + u.y;

	return result;
}
vector2 operator*(const vector2& v, const float& f)
{
	vector2 result;

	result.x = v.x * f;
	result.y = v.y * f;

	return result;
}
bool operator==(const vector2& v, const vector2& u)
{
	if (v.x == u.x && v.y == u.y) return true;
	return false;
}
float length(const vector2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}