#include <iostream>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <glm/gtc/epsilon.hpp>

const float THRESHOLD = 1.0e-5f;

// Triple Product Preposition
static void tripleProductPreposition() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 5);

	glm::ivec3 ai(dis(gen), dis(gen), dis(gen));
	glm::ivec3 bi(dis(gen), dis(gen), dis(gen));
	glm::ivec3 ci(dis(gen), dis(gen), dis(gen));

	glm::vec3 a = glm::vec3(ai);
	glm::vec3 b = glm::vec3(bi);
	glm::vec3 c = glm::vec3(ci);

	glm::vec3 left = glm::cross(a, glm::cross(b, c));
	glm::vec3 right = b * glm::dot(a, c) - c * glm::dot(a, b);

	bool equal = true;
	for (int i = 0; i < 3; ++i)
		if (!glm::epsilonEqual(left[i], right[i], THRESHOLD))
			equal = false;

	std::cout << (equal ? "true" : "false") << std::endl << std::endl;
	assert(equal && "Triple Product Preposition failed!");
}

// Create Coordinate Frame
static void createCoordinateFrame(const glm::vec3& view, const glm::vec3& up) {

	glm::vec3 v = glm::normalize(view);
	glm::vec3 w = glm::normalize(glm::cross(up, v));
	glm::vec3 u = glm::cross(v, w);

	std::cout << "View Vector: (" << view.x << ", " << view.y << ", " << view.z << "), "
		<< "Up Vector: (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;

	glm::vec3 cross = glm::cross(view, up);
	if (glm::length(cross) < THRESHOLD) {
		std::cout << "Error: 'view' and 'up' vectors are colinear. Cannot create coordinate frame." << std::endl << std::endl;
		return;
	}

	std::cout << "Coordinate Frame:" << std::endl
		<< "u: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl
		<< "v: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl
		<< "w: (" << w.x << ", " << w.y << ", " << w.z << ")" << std::endl << std::endl;
}

// Rodrigues Rotation Formula Vector
static void rodriguesRotationFormula(const glm::vec3& v, const glm::vec3& axis, float angle_deg, const char* expected) {
	float angle = glm::radians(angle_deg);
	glm::vec3 a = glm::normalize(axis);
	glm::vec3 v_rotated = v * cos(angle) + glm::cross(a, v) * sin(angle) + a * glm::dot(a, v) * (1 - cos(angle));

	std::cout << "Input Vector: (" << v.x << ", " << v.y << ", " << v.z << "), "
		<< "Axis: (" << axis.x << ", " << axis.y << ", " << axis.z << "), "
		<< "Angle: " << angle_deg << " degrees" << std::endl;
	std::cout << "Expected Output: " << expected << std::endl;
	std::cout << "Computed Output: (" << v_rotated.x << ", " << v_rotated.y << ", " << v_rotated.z << ")" << std::endl << std::endl;
}

// Rodrigues Rotation Formula Matrix
static void rodriguesRotationMatrix(const glm::vec3& v, const glm::vec3& axis, float angle_deg, const glm::vec3& expected) {
	float angle = glm::radians(angle_deg);
	glm::vec3 a = glm::normalize(axis);
	glm::mat3 I(1.0f);
	glm::mat3 A(0.0f);
	A[0][1] = -a.z; A[0][2] = a.y;
	A[1][0] = a.z;  A[1][2] = -a.x;
	A[2][0] = -a.y; A[2][1] = a.x;

	glm::mat3 R = I + sin(angle) * A + (1 - cos(angle)) * A * A;
	glm::vec3 result = R * v;

	std::cout << "Input: (" << v.x << ", " << v.y << ", " << v.z << "), "
		<< "Axis: (" << axis.x << ", " << axis.y << ", " << axis.z << "), "
		<< "Angle: " << angle_deg << " degrees" << std::endl;
	std::cout << "Expected Output: (" << expected.x << ", " << expected.y << ", " << expected.z << ")" << std::endl;
	std::cout << "Computed Output: (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl << std::endl;
}

// Transpose Property
static void transposeProperty() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-5, 5);

	glm::mat3 A, B;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			A[i][j] = static_cast<float>(dis(gen));
			B[i][j] = static_cast<float>(dis(gen));
		}

	glm::mat3 AB_T = glm::transpose(A * B);
	glm::mat3 BT_AT = glm::transpose(B) * glm::transpose(A);

	bool equal = true;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!glm::epsilonEqual(AB_T[i][j], BT_AT[i][j], THRESHOLD))
				equal = false;

	std::cout << (equal ? "true" : "false") << std::endl << std::endl;
	assert(equal && "Transpose Property failed!");
}

// Inverse Property
static void inverseProperty() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-5, 5);

	glm::mat3 A, B;
	do {
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j) {
				A[i][j] = static_cast<float>(dis(gen));
				B[i][j] = static_cast<float>(dis(gen));
			}
	} while (glm::determinant(A) == 0.0f || glm::determinant(B) == 0.0f);

	glm::mat3 AB_inv = glm::inverse(A * B);
	glm::mat3 B_inv_A_inv = glm::inverse(B) * glm::inverse(A);

	bool equal = true;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!glm::epsilonEqual(AB_inv[i][j], B_inv_A_inv[i][j], THRESHOLD))
				equal = false;

	std::cout << (equal ? "true" : "false") << std::endl << std::endl;
	assert(equal && "Inverse Property failed!");
}

// Main function
int main() {
	std::cout << "1 Triple Product Preposition holds:" << std::endl;
	tripleProductPreposition();

	std::cout << "2 Coordinate Frames:" << std::endl << std::endl;
	createCoordinateFrame(glm::vec3(2.0f, 3.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	createCoordinateFrame(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	createCoordinateFrame(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	std::cout << "3 Rodrigues Rotation Formula (Vector):" << std::endl << std::endl;
	rodriguesRotationFormula(glm::vec3(1.0f, 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, "(-3, 1, 0)");
	rodriguesRotationFormula(glm::vec3(1.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, "(-1, 0, 3)");
	rodriguesRotationFormula(glm::vec3(0.0f, 2.0f, 4.0f), glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, "(0, -4, 2)");

	std::cout << "4 Rodrigues Rotation Formula (Matrix):" << std::endl << std::endl;
	std::cout << "Vector: (5, 0, 0)" << std::endl;
	rodriguesRotationMatrix(glm::vec3(5, 0, 0), glm::vec3(0, 1, 0), 90.0f, glm::vec3(0, 0, 5));
	rodriguesRotationMatrix(glm::vec3(5, 0, 0), glm::vec3(0, 0, 1), 90.0f, glm::vec3(0, -5, 0));
	rodriguesRotationMatrix(glm::vec3(5, 0, 0), glm::vec3(1, 0, 0), 90.0f, glm::vec3(5, 0, 0));

	std::cout << "Vector: (0, 3, 0)" << std::endl;
	rodriguesRotationMatrix(glm::vec3(0, 3, 0), glm::vec3(0, 1, 0), 90.0f, glm::vec3(0, 3, 0));
	rodriguesRotationMatrix(glm::vec3(0, 3, 0), glm::vec3(0, 0, 1), 90.0f, glm::vec3(3, 0, 0));
	rodriguesRotationMatrix(glm::vec3(0, 3, 0), glm::vec3(1, 0, 0), 90.0f, glm::vec3(0, 0, -3));

	std::cout << "Vector: (0, 0, -4)" << std::endl;
	rodriguesRotationMatrix(glm::vec3(0, 0, -4), glm::vec3(0, 1, 0), 90.0f, glm::vec3(4, 0, 0));
	rodriguesRotationMatrix(glm::vec3(0, 0, -4), glm::vec3(0, 0, 1), 90.0f, glm::vec3(0, 0, -4));
	rodriguesRotationMatrix(glm::vec3(0, 0, -4), glm::vec3(1, 0, 0), 90.0f, glm::vec3(0, -4, 0));

	std::cout << "5 Transpose Property holds:" << std::endl;
	transposeProperty();

	std::cout << "6 Inverse Property holds:" << std::endl;
	inverseProperty();

	return 0;
}
