#include <algorithm>

#include "pch.h"
#include "CppUnitTest.h"

#include "Simulation.h"

#include "Application.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CollisionTesting
{
	TEST_CLASS(Projection)
	{
	public:
		
		TEST_METHOD(RigidbodyProjection)
		{
			glm::vec2 position = glm::vec2(100, 100);
			glm::vec2 zero = glm::vec2(0, 0);
			std::vector<glm::vec2> vertices = std::vector<glm::vec2>();

			glm::vec2 vert1 = glm::vec2(-10, -10);
			glm::vec2 vert2 = glm::vec2(-9, 10);
			glm::vec2 vert3 = glm::vec2(9, 10);
			glm::vec2 vert4 = glm::vec2(10, -10);
			vertices.push_back(vert1);
			vertices.push_back(vert2);
			vertices.push_back(vert3);
			vertices.push_back(vert4);

			Rigidbody rigidbody = Rigidbody(vertices, position, zero, zero, 1, ImVec4(), false);


			glm::vec2 axis = glm::vec2(1, 0);

			// Expected result:
			float min = glm::dot(axis, vert1 + position);
			float max = glm::dot(axis, vert4 + position);

			glm::vec2 projection = rigidbody.project(axis);

			Assert::IsTrue(projection == glm::vec2(min, max));
		}

		TEST_METHOD(CircleProjection) {
			glm::vec2 position = glm::vec2(100, 100);
			glm::vec2 zero = glm::vec2(0, 0);
			float radius = 50;
			Circle circle = Circle(50, position, zero, zero, 1, ImVec4(), false);

			glm::vec2 axis = glm::normalize(glm::vec2(1, 1));

			// Expected result:
			float min = glm::dot(axis, position) - radius;
			float max = glm::dot(axis, position) + radius;

			glm::vec2 projection = circle.project(axis);

			Assert::IsTrue(projection == glm::vec2(min, max));

		}
	};

	TEST_CLASS(FindingAxes) {

	public:

		TEST_METHOD(RigidbodyFindAxes) {
			glm::vec2 position = glm::vec2(100, 100);
			glm::vec2 zero = glm::vec2(0, 0);
			std::vector<glm::vec2> vertices = std::vector<glm::vec2>();

			glm::vec2 vert1 = glm::vec2(-10, -10);
			glm::vec2 vert2 = glm::vec2(-10, 10);
			glm::vec2 vert3 = glm::vec2(10, 10);
			glm::vec2 vert4 = glm::vec2(10, -10);
			vertices.push_back(vert1);
			vertices.push_back(vert2);
			vertices.push_back(vert3);
			vertices.push_back(vert4);

			Rigidbody rb = Rigidbody(vertices, position, zero, zero, 1, ImVec4(), false);

			std::vector<glm::vec2> axes = rb.getAxes();
			bool axisIn[4] = { false, false, false, false };

			for (auto axis : axes) {
				if (glm::vec2(1, 0) == axis) {
					axisIn[0] = true;
				}
				else if (glm::vec2(0, -1) == axis) {
					axisIn[1] = true;
				}
				else if (glm::vec2(-1, 0) == axis) {
					axisIn[2] = true;
				}
				else if (glm::vec2(0, 1) == axis) {
					axisIn[3] = true;
				}
			}

			for (auto in : axisIn) {
				if (!in)
					Assert::Fail();
			}
		}

		TEST_METHOD(CircleFindAxes) {
			glm::vec2 position = glm::vec2(100, 100);
			glm::vec2 zero = glm::vec2(0, 0);

			float radius = 50;

			Circle circle = Circle(radius, position, zero, zero, 1, ImVec4(), false);

			std::vector<glm::vec2> axes = circle.getAxes();

			std::vector<glm::vec2> expected_axes = std::vector<glm::vec2>();
			int noAxes = axes.size();

			// Make sure that at least 1 axis was returned
			Assert::AreNotSame(noAxes, 0);

			for (float angle = 0; angle < 2 * 3.141; angle += 2 * 3.141 / (noAxes - 1)) {
				expected_axes.push_back(glm::normalize(glm::vec2(cos(angle), sin(angle))));
			}

			for (int i = 0; i < axes.size(); i++) {
				Assert::IsTrue(axes[i] == expected_axes[i]);
			}

		}

	};

	TEST_CLASS(CollisionDetection) {
	public:

		TEST_METHOD(IsColliding) {
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(125, 125);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, zero, zero, 1, ImVec4(), false);
			Square square2 = Square(sideLength, pos2, zero, zero, 1, ImVec4(), false);
			
			Simulation sim = Simulation();

			Assert::IsTrue(sim.checkCollision(&square1, &square2) != glm::vec2(0 ,0));
		}

		TEST_METHOD(IsNotColliding) {
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(300, 300);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, zero, zero, 1, ImVec4(), false);
			Square square2 = Square(sideLength, pos2, zero, zero, 1, ImVec4(), false);

			Simulation sim = Simulation();

			Assert::IsTrue(sim.checkCollision(&square1, &square2) == glm::vec2(0, 0));
		}

		TEST_METHOD(FindingMTV) {

			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(100, 140);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, zero, zero, 1, ImVec4(), false);
			Square square2 = Square(sideLength, pos2, zero, zero, 1, ImVec4(), false);

			Simulation sim = Simulation();

			Assert::IsTrue(sim.checkCollision(&square1, &square2) == glm::vec2(0, -10));
		}
	};

	TEST_CLASS(CollisionResolution) {
	public:

		TEST_METHOD(PositionCorrectionNN) {
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(100, 140);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, zero, zero, 1, ImVec4(), false);
			Square square2 = Square(sideLength, pos2, zero, zero, 1, ImVec4(), false);

			Simulation sim = Simulation();

			glm::vec2 MTV = glm::vec2(0, -10);

			sim.positionCorrection(&square1, &square2, MTV);

			glm::vec2 expected_pos1 = glm::vec2(100, 95);
			glm::vec2 expected_pos2 = glm::vec2(100, 145);

			Assert::IsTrue(expected_pos1 == square1.getPosition());
			Assert::IsTrue(expected_pos2 == square2.getPosition());

		}

		TEST_METHOD(PositionCorrectionSN) {
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(100, 140);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, zero, zero, 1, ImVec4(), true);
			Square square2 = Square(sideLength, pos2, zero, zero, 1, ImVec4(), false);

			Simulation sim = Simulation();

			glm::vec2 MTV = glm::vec2(0, -10);

			sim.positionCorrection(&square1, &square2, MTV);

			glm::vec2 expected_pos1 = glm::vec2(100, 100);
			glm::vec2 expected_pos2 = glm::vec2(100, 150);

			Assert::IsTrue(expected_pos1 == square1.getPosition());
			Assert::IsTrue(expected_pos2 == square2.getPosition());

		}

		TEST_METHOD(ImpulseCalculationNN) {
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(100, 140);
			glm::vec2 vel1 = glm::vec2(100, 100);
			glm::vec2 vel2 = glm::vec2(100, -100);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, vel1, zero, 1, ImVec4(), false);
			Square square2 = Square(sideLength, pos2, vel2, zero, 1, ImVec4(), false);

			Simulation sim = Simulation();

			glm::vec2 MTV = glm::vec2(0, -10);

			sim.impulseCalculation(&square1, &square2, MTV);

			square1.update(0.016);
			square2.update(0.016);

			glm::vec2 expected_vel1 = glm::vec2(100, -100);
			glm::vec2 expected_vel2 = glm::vec2(100, 100);

			Assert::IsTrue(expected_vel1 == square1.getVelocity());
			Assert::IsTrue(expected_vel2 == square2.getVelocity());
		}

		TEST_METHOD(ImpulseCalculationSN) {
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(100, 140);
			glm::vec2 vel2 = glm::vec2(100, -100);
			glm::vec2 zero = glm::vec2(0, 0);

			float sideLength = 50;

			Square square1 = Square(sideLength, pos1, zero, zero, 1, ImVec4(), true);
			Square square2 = Square(sideLength, pos2, vel2, zero, 1, ImVec4(), false);

			Simulation sim = Simulation();

			glm::vec2 MTV = glm::vec2(0, -10);

			sim.impulseCalculation(&square1, &square2, MTV);

			square1.update(0.016);
			square2.update(0.016);

			glm::vec2 expected_vel1 = glm::vec2(0, 0);
			glm::vec2 expected_vel2 = glm::vec2(100, 100);

			Assert::IsTrue(expected_vel1 == square1.getVelocity());
			Assert::IsTrue(expected_vel2 == square2.getVelocity());
		}
	};
}

namespace SoftbodyTesting {

	TEST_CLASS(Springs) {

		TEST_METHOD(SpringUpdate) {

			float radius = 5;
			glm::vec2 pos1 = glm::vec2(100, 100);
			glm::vec2 pos2 = glm::vec2(100, 200);
			glm::vec2 zero = glm::vec2(0, 0);
			float mass = 1;

			std::shared_ptr<Circle> c1 = std::make_shared<Circle>(Circle(radius, pos1, zero, zero, mass, ImVec4(), false));
			std::shared_ptr<Circle> c2 = std::make_shared<Circle>(Circle(radius, pos2, zero, zero, mass, ImVec4(), false));

			float naturalLength = 50;
			float springConstant = 30;
			float dampeningFactor = 0.5;

			Spring spring = Spring(c1, c2, naturalLength, springConstant, dampeningFactor);

			float dt = 0.016;

			spring.update(0.016);
			c1->update(dt);
			c2->update(dt);

			float extension = 50;
			glm::vec2 force = extension * springConstant * glm::vec2(0, 1);

			float rMass = 1 / mass + 1 / mass;

			glm::vec2 vel1 = (force * 0.5f * dt) / mass;
			glm::vec2 vel2 = (-force * 0.5f * dt) / mass;

			glm::vec2 expectedPosition1 = pos1 + vel1 * dt;
			glm::vec2 expectedPosition2 = pos2 + vel2 * dt;

			Assert::IsTrue(expectedPosition1 == c1->getPosition() && expectedPosition2 == c2->getPosition());

		}

	};

}