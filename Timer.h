#pragma once
#include "StepTimer.h"

// <ゲームタイマー>>
class Timer final
{
private:
	// 開始時間
	float start_time;
	// 終了時間
	float last_time;
	// 一時停止
	bool paused;
	// 総合時間
	float total;
	// タイマー
	const DX::StepTimer& m_timer;

public:
	// <タイマー作成>
	Timer(const DX::StepTimer& timer);

private:
	// <時刻取得>
	float GetNow();

public:
	// <タイマー時間>
	float GetTime();

	// <タイマー一時停止中>
	bool IsPaused();

	// <タイマー停止>
	Timer& Pause();

	// <タイマーリセット>
	Timer& Reset();

	// <タイマー再開>
	Timer& Resume();

	// <タイマーセット>
	Timer& Set(float new_time);

	// <タイマー残り時間セット>
	Timer& SetRemaining(float remaining);

	// <タイマー残り時間リセット>
	Timer& ResetRemaining();

	// <トータル時間セット>
	Timer& SetTotal(float total);

	// <トータル時間セット&残り時間リセット&再開>
	Timer& Start(float total);

	// <トータル時間取得>
	float GetTotal();

	// <進捗取得>
	float GetProgress();

	// <タイマー残り時間>
	float GetRemaining();

	// <タイマー経過時間>
	float GetElapsed();

	// <タイマー終了判定>
	bool IsFinished();
};