#include "Keyboard.h"

bool Keyboard::keyIsPressed(unsigned char keycode) const noexcept
{
    return keyStates[keycode];
}

std::optional<Keyboard::Event> Keyboard::readKey() noexcept
{
    if (keyBuffer.size() > 0u) {
        Keyboard::Event e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    return {};
}

bool Keyboard::keyIsEmpty() const noexcept
{
    return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keyBuffer = std::queue<Event>();
}

void Keyboard::onKeyPressed(unsigned char keycode) noexcept
{
    keyStates[keycode] = true;
    keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    trimBuffer(keyBuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) noexcept
{
    keyStates[keycode] = false;
    keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    trimBuffer(keyBuffer);
}

void Keyboard::clearState() noexcept
{
    keyStates.reset();
}

template<typename T>
void Keyboard::trimBuffer( std::queue<T>& buffer ) noexcept
{
	while( buffer.size() > bufferSize )
	{
		buffer.pop();
	}
}
