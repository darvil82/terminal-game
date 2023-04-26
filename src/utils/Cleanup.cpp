import <functional>;

export module utils.cleanup;


namespace utils {
export class Cleanup {
	std::function<void()> func;

public:
	Cleanup(std::function<void()> f) : func {f} { }

	~Cleanup() {
		this->func();
	}
};


}
