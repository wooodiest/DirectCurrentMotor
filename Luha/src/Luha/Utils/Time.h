#pragma once

#include "Luha/Utils/Timestep.h"

#include <functional>
#include <iostream>

namespace Luha {

	enum class TimeUnit
	{
		ms = 1,
		s
	};

	class Clock
	{
	public:
		template<typename Func, typename... Args>
		Clock(Timestep ts, Func&& callback, Args&&... args)
			: m_Time(ts), m_Callback([=]() { callback(std::forward<Args>(args)...); })
		{
		}
		~Clock() = default;

		void OnUpdate();

		bool IsActive() const { return m_Active; }
		Timestep GetCurrentTime() const { return m_CurrentTime; }
		void Stop() { m_Active = false; }
		void Resume() { m_Active = true; }
		void Reset() { m_CurrentTime = 0.0f; }
		void Restart() { Reset(); Resume(); }

	private:
		Timestep m_Time = 0.0f;
		Timestep m_CurrentTime = 0.0f;
		bool m_Active = true;
		std::function<void()> m_Callback;

	};

	class RepeatClock
	{
	public:
		template<typename Func, typename... Args>
		RepeatClock(Timestep ts, bool callOnStartUp, Func&& callback, Args&&... args)
			: m_Time(ts), m_Callback([=]() { callback(std::forward<Args>(args)...); })
		{
			if (callOnStartUp)
			{
				m_Callback();
				m_CallCount++;
			}
		}
		~RepeatClock() = default;

		void OnUpdate();

		bool IsActive() const { return m_Active; }
		uint32_t GetCallCount() const { return m_CallCount; }

		Timestep GetCurrentTime() const { return m_CurrentTime; }
		void Stop() { m_Active = false; }
		void Resume() { m_Active = true; }
		void Reset() { m_CurrentTime = 0.0f; }
		void Restart() { Reset(); Resume(); }

	private:
		Timestep m_Time = 0.0f;
		Timestep m_CurrentTime = 0.0f;
		bool m_Active = true;
		uint32_t m_CallCount = 0;
		std::function<void()> m_Callback;

	};

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Timer::Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name, TimeUnit unit = TimeUnit::s)
			: m_Name(name), m_Unit(unit) {}
		~ScopedTimer()
		{
			float time = 0.0f;
			char* _unit = "s";
			switch (m_Unit)
			{
				case TimeUnit::ms: time = m_Timer.ElapsedMillis(); _unit = "ms" ; break;
				case TimeUnit::s: time = m_Timer.Elapsed(); _unit = "s"; break;
				default: time = m_Timer.Elapsed(); _unit = "s"; break;
			} 
			LH_INFO("[TIMER] {0} - {1}{2}", m_Name, time, _unit);
		}
	private:
		const char* m_Name;
		TimeUnit m_Unit;
		Timer m_Timer;
	};

}