#include <string>

class json_context {
    public:
    std::string json;
    
    char* stack;
    size_t size, top;
};

static void* json_context_pop(json_context* c, size_t size)
{
    //std::cout << "Running json_context_pop()\n";
    assert(c->top >= size);
    //std::cout << "This line...\n";
    return c->stack + (c->top -= size);
}

int main()
{
    std::string json = "good";
    json_context c;
    c.json = json;
    c.stack = nullptr;
    c.size = c.top = 0;
    json_context_pop(&c, 0);
}