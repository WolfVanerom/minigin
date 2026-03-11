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

	class TrashTheCashComponent final : public Component
	{
	public:
		explicit TrashTheCashComponent(GameObject* pOwner);
		virtual ~TrashTheCashComponent() = default;
		TrashTheCashComponent(const TrashTheCashComponent& other) = delete;
		TrashTheCashComponent(TrashTheCashComponent&& other) noexcept = delete;
		TrashTheCashComponent& operator=(const TrashTheCashComponent& other) = delete;
		TrashTheCashComponent& operator=(TrashTheCashComponent&& other) noexcept = delete;
		virtual void Update(float deltaTime);
		virtual void Render() const override;
	private:
		mutable PerformanceTestResult m_cacheTestResult;
		mutable bool m_isAltTest = false;
		void RunCashePerformanceTest(TestType testType, int newBufferSize) const;
		void RenderExercise1TestWindow() const;
		void RenderExercise2TestWindow() const;
	};
}