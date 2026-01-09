#pragma once

class Timer {
public:
    Timer();

    void update(float dt);

    int getMinutes() const;
    int getSeconds() const;
    float getTime() const;

    bool isFinished() const;

private:
    float elapsed;          // secondi
    const float maxTime;    // 30 minuti
};
