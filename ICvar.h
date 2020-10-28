#pragma once
#include "ConVar.h"

//me
class IAppSystem
{
public:
    virtual void func0() = 0;
    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void func4() = 0;
    virtual void func5() = 0;
    virtual void func6() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void func9() = 0;
};

class ICvar : public IAppSystem
{
public:
	virtual void func10() = 0;
	virtual void func11() = 0;
	virtual void func12() = 0;
	virtual void func13() = 0;
	virtual ConVar* FindVar(const char* var_name) = 0; // 14
	virtual void func15() = 0;
	virtual void func16() = 0;
	virtual void func17() = 0;
	virtual void func18() = 0;
	virtual void func19() = 0;
	virtual void func20() = 0;
};