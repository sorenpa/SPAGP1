


class MpkFileReader
{
public:
	MpkFileReader();
	~MpkFileReader();

	int open(char *path);
	int read();
private:
	int uncompress();
};

