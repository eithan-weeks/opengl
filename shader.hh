#ifndef SHADER_HH
#define SHADER_HH

struct shader {
	unsigned int id;

	shader(char const*, const char*);
	~shader();

	void use();

	void setBool(char const*, bool) const;
	void setInt(char const*, int) const;
	void setFloat(char const*, float) const;
};

#endif
