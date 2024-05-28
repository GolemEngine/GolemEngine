#pragma once


class Toolbar
{
private:
    // Private constructors and destructor to make static class
    Toolbar() = delete;
    Toolbar(Toolbar& _other) = delete;
    Toolbar(Toolbar&& _other) = delete;
    Toolbar& operator=(Toolbar& _other) = delete;
    Toolbar& operator=(Toolbar&& _other) = delete;
    void operator=(const Toolbar&) = delete;
    ~Toolbar() = delete;

public:
    static void Update();
};