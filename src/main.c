#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>

#define DISCORD_ENDPOIT "https://discord.com/api/"
#define API_VERSION "v10/"
#define USE_MANAGER "users/@me"

void print_ascii_art(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);
}

struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        printf("Erro de _MEMORY_ALLOC\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <token>\n", argv[0]);
        return 1;
    }

    const char *token = argv[1];

    const char *filename = "ascii/funny.txt";
    print_ascii_art(filename);

    CURL *curl;
    CURLcode response;

    char constructApiUrl[256];
    char auth_header[256];

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (curl)
    {
        sprintf(constructApiUrl, "%s%s%s", DISCORD_ENDPOIT, API_VERSION, USE_MANAGER);
        sprintf(auth_header, "Authorization: Bot %s", token);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, auth_header);

        printf("full api url: %s\n", constructApiUrl);

        curl_easy_setopt(curl, CURLOPT_URL, constructApiUrl);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        response = curl_easy_perform(curl);

        if (response != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s", curl_easy_strerror(response));
        }
        else
        {
            char *notfound = "_not_found";

            typedef struct
            {
                char *name;
                char *id;
                char *description;
            } BotInfo;

            BotInfo bot;

            struct json_object *parsed_json = json_tokener_parse(chunk.memory);
            struct json_object *username, *id, *description;

            if (json_object_object_get_ex(parsed_json, "username", &username))
            {
                bot.name = strdup(json_object_get_string(username));
            }
            else
            {
                bot.name = strdup(notfound);
            }

            if (json_object_object_get_ex(parsed_json, "id", &id))
            {
                bot.id = strdup(json_object_get_string(id));
            }
            else
            {
                bot.id = strdup(notfound);
            }
            if (json_object_object_get_ex(parsed_json, "description", &description))
            {
                bot.description = strdup(json_object_get_string(description));
            }
            else
            {
                bot.description = strdup(notfound);
            }

            json_object_put(parsed_json);

            printf("name: %s\ndesc: %s\nid: %s\ntoken: %s\n\n", bot.name, bot.description, bot.id, token);
            printf("bruh\n");
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    curl_global_cleanup();
    return 0;
}