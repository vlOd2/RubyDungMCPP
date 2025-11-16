#pragma once

using namespace System;
using namespace System::Diagnostics;

constexpr long NS_TO_S = 1000000000;

public ref class RDTimer {
private:
	float ticksPerSecond;
	long lastTime;
	static System::DateTime epoch = System::DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::Utc); 

public:
	int ticks;
	float a;
	float timeScale = 1.0F;
	float fps = 0.0F;
	float passedTime = 0.0F;

	RDTimer(float tps) {
		this->ticksPerSecond = tps;
		this->lastTime = MilliTime();
	}

	static long NanoTicks() {
		return Stopwatch::GetTimestamp() * NS_TO_S / Stopwatch::Frequency;
	}

	static long MilliTime() {
		return (long)(DateTime::UtcNow - epoch).TotalMilliseconds;
	}

	static long SecondTime() {
		return (long)(DateTime::UtcNow - epoch).TotalSeconds;
	}

	void AdvanceTime() {
		long now = this->NanoTicks();
		long passedNs = now - this->lastTime;
		this->lastTime = now;

		if (passedNs < 0L) {
			passedNs = 0L;
		}
		if (passedNs > NS_TO_S) {
			passedNs = NS_TO_S;
		}

		this->fps = (float)(1000000000L / passedNs);
		this->passedTime += (float)passedNs * this->timeScale * this->ticksPerSecond / 1.0E9F;
		this->ticks = (int)this->passedTime;
		if (this->ticks > 100) {
			this->ticks = 100;
		}

		this->passedTime -= (float)this->ticks;
		this->a = this->passedTime;
	}
};