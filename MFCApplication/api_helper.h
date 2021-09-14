
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/allocators.h"

#include <iostream>
#include <string>
#include "curl/curl.h"
using namespace rapidjson;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static void save_to_file(std::string apiURL,std::string id,std::string &path)
{


	/*static std::ofstream file(id + ".jpg", std::ios::binary);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&file));*/
	CURL *curl;
	FILE *fp = fopen("D:\\Projects\\citi_assesment\\info.jpg", "wb");
	CURLcode res;
	//char *url = "http://localhost/aaa.txt";
	////std::string pa="D:\\Projects\\citi_assesment\\info.jpg";// = /*("res/") + */id + ".jpg";
	////const int n = pa.length();
	////path = pa;
	////// declaring character array
	////char char_array[100];

	// copying the contents of the
	// string to char array
	//strcpy(char_array, pa.c_str());
	//char outfilename[FILENAME_MAX] = (char)pa;
	curl = curl_easy_init();
	//struct curl_slist *header = NULL;
	//header = curl_slist_append(header, "Content-Type: image/jpeg");
	//header = curl_slist_append(header, "Accept: image/jpeg");
	if (curl)
	{
		//fp = fopen(char_array, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, apiURL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
	//return "";
} 
//





//
static std::string GetApiData(std::string apiURL)
{
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, apiURL.c_str());
		struct curl_slist *header = NULL;
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
		header = curl_slist_append(header, "app-id:613c5c21a78bc86d7fed70da");
		header = curl_slist_append(header, "Content-Type: application/json");
		header = curl_slist_append(header, "Accept: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		std::cout << readBuffer << std::endl;
	}
	return readBuffer;
}