#ifndef flags_h
#define flags_h
template<typename T>
class flags
{
public:
	flags() :value(0) {}
	T value;
	inline const T& get()const { return value; }
	inline void set(const T& v) { value = v; }
	inline bool chk(const T& v)const { return (value&v) != 0; }
	inline bool chk(const flags<T>& v)const { return (value&v.get()) != 0; }
	inline void add(const T& v) { value |= v; }
	inline void del(const T& v) { value &= (~v); }
	inline void reset() { value = 0; }
};
#endif // !flags_hpp

