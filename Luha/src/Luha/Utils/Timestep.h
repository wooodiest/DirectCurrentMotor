#pragma once

//TODO: Add fixed delta time

namespace Luha {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {}

		operator float() { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

		Timestep operator+(const Timestep& other) const { return Timestep(m_Time + other.m_Time); }
		Timestep operator-(const Timestep& other) const { return Timestep(m_Time - other.m_Time); }
		Timestep operator*(const Timestep& other) const { return Timestep(m_Time * other.m_Time); }
		Timestep operator/(const Timestep& other) const { return Timestep(m_Time / other.m_Time); }
		Timestep operator*(float scalar) const { return Timestep(m_Time * scalar); }
		Timestep operator/(float scalar) const { return Timestep(m_Time / scalar); }

		friend Timestep operator*(float scalar, const Timestep& timestep) { return Timestep(scalar * timestep.m_Time); }
		friend Timestep operator/(float scalar, const Timestep& timestep) { return Timestep(scalar / timestep.m_Time); }

		bool operator==(const Timestep& other) const { return m_Time == other.m_Time; }
		bool operator!=(const Timestep& other) const { return !(*this == other); }
		bool operator<(const Timestep& other) const { return m_Time < other.m_Time; }
		bool operator>(const Timestep& other) const { return m_Time > other.m_Time; }
		bool operator<=(const Timestep& other) const { return m_Time <= other.m_Time; }
		bool operator>=(const Timestep& other) const { return m_Time >= other.m_Time; }

		bool operator==(float other) const { return m_Time == other; }
		bool operator!=(float other) const { return m_Time != other; }
		bool operator<(float other) const { return m_Time < other; }
		bool operator>(float other) const { return m_Time > other; }
		bool operator<=(float other) const { return m_Time <= other; }
		bool operator>=(float other) const { return m_Time >= other; }

		Timestep& operator+=(const Timestep& other) { m_Time += other.m_Time; return *this; }
		Timestep& operator-=(const Timestep& other) { m_Time -= other.m_Time; return *this; }
		Timestep& operator*=(const Timestep& other) { m_Time *= other.m_Time; return *this; }
		Timestep& operator/=(const Timestep& other) { m_Time /= other.m_Time; return *this; }
		Timestep& operator*=(float scalar) { m_Time *= scalar; return *this; }
		Timestep& operator/=(float scalar) { m_Time /= scalar; return *this; }

		Timestep& operator=(const Timestep& other) { m_Time = other.m_Time; return *this; }
		Timestep& operator=(float other) { m_Time = other; return *this; }

	private:	
		float m_Time;
	};

}