#pragma once

template <typename T>
class ISingleton
{
public:
	static T& GetInst()
	{
		static T Instance;
		return Instance;
	}

protected:
	ISingleton() {} = default;
	virtual ~ISingleton() {} = default;

	// ���� �� �̵� ����
	ISingleton(const ISingleton&) = delete;
	ISingleton& operator=(const ISingleton&) = delete;
	ISingleton(ISingleton&&) = delete;
	ISingleton& opreator = (const ISingleton&&) = delete;
};

