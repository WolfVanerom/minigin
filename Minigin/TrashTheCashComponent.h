#pragma once
#include "Component.h"
#include <vector>
#include <string>

namespace dae
{
	struct PerformanceTestResult
	{
		std::string testName;
		std::vector<int> steps;
		std::vector<float> timings;
		bool hasData = false;
	};

	enum TestType
	{
		CacheTest,
		GameObject3DTest,
		GameObject3DAltTest
	};

	class TrashTheCashComponent final : Component
	{
	public:
		TrashTheCashComponent() = default;
		virtual ~TrashTheCashComponent() = default;
		TrashTheCashComponent(const TrashTheCashComponent& other) = delete;
		TrashTheCashComponent(TrashTheCashComponent&& other) noexcept = delete;
		TrashTheCashComponent& operator=(const TrashTheCashComponent& other) = delete;
		TrashTheCashComponent& operator=(TrashTheCashComponent&& other) noexcept = delete;
		virtual void Update(float deltaTime);
		virtual void Render() const override;
	private:
		PerformanceTestResult m_cacheTestResult;
		void RunCashePerformanceTest(TestType testType, int newBufferSize);
	};
}