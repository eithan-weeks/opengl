#ifndef SHADER_HH
#define SHADER_HH

class shader {
public:
	shader(char const*, const char*);
	~shader();

	void use();

	void setBool(char const*, bool) const;
	void setInt(char const*, int) const;
	void setFloat(char const*, float) const;

private:
	unsigned int id;
};

#endif
