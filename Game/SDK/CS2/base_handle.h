#pragma once

class CBasePlayer;
class CHandle;

class CHandle {
public:
	[[nodiscard]] std::uint32_t get_entry_index() const {
		return index_ & 0x7FFF;
	}

	[[nodiscard]] bool is_valid() const {
		return index_ > 0 && index_ != 0xFFFFFFFF;
	}

	[[nodiscard]] std::uint32_t index() const {
		return index_;
	}

	[[nodiscard]] bool operator==( const CHandle& other ) const {
		return index_ == other.index_;
	}

	[[nodiscard]] bool operator!=( const CHandle& other ) const {
		return index_ != other.index_;
	}

private:
	std::uint32_t index_;
};