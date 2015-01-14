#include <stdio.h>
#include <string>

int ReadFile(std::string filename, std::string &strResult)
{

	//note file path need adjus
	FILE * fp = fopen(filename.c_str(), "r");
	if (NULL != fp)
	{
		char buf[1000] = {0};
		int nRealRead = fread(buf, 1, 1000, fp);

		while (nRealRead>0)
		{
			strResult.append(buf);
			nRealRead = fread(buf, 1, 1000, fp);
		}

		fclose(fp);
	}
	else
	{
		return -1;
	}
	return 0;
}
int WriteFile(std::string filename, std::string &strContent)
{

	FILE * fp = fopen(filename.c_str(), "w");
	if (NULL != fp)
	{
		fwrite(strContent.c_str(), 1, strContent.length(), fp);
		fclose(fp);
	}
	else
	{
		return -1;
	}
	return 0;
}