#pragma once

#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Press,
			Release
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event(Type type, unsigned char code) noexcept :
			type(type), code(code)
		{}
		bool isPress() const noexcept {
			return type == Type::Press;
		}
		bool isRelease() const noexcept {
			return type == Type::Release;
		}
		unsigned char getCode() const noexcept {
			return code;
		}
	};
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool keyIsPressed(unsigned char keycode) const noexcept;
	std::optional<Event> readKey() noexcept;
	bool keyIsEmpty() const noexcept;
	void FlushKey() noexcept;
private:
	void onKeyPressed(unsigned char keycode) noexcept;
	void onKeyReleased(unsigned char keycode) noexcept;
	void clearState() noexcept;
	template<typename T>
	static void trimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
};
