#include <sys/mman.h>
#include <iostream>

class Coalesce
{
public:
    struct BlockData
    {
        BlockData(BlockData* _next,
                  BlockData* _prev,
                  void* _memory,
                  size_t _size,
                  bool _with_value)
        {
            next = _next;
            prev = _prev;
            memory = _memory;
            size = _size;
        }

        BlockData* next;
        BlockData* prev;
        void * memory;
        size_t size;
        bool with_value;
    };

    struct PageNode
    {
        PageNode(PageNode* _next, void* _memory)
        {
            next = _next;
            memory = _memory;
        }

        PageNode * next;
        void * memory;
        size_t count_of_blocks;
        BlockData* first_block_data;
    };

    Coalesce()
    {
        _count_of_pages = 1;
    }

    ~Coalesce()
    {
        if (_inited == true)
        {
            destroy();
        }
        
    }

    void init()
    {
        _inited = true;
        
        void* map = mmap(NULL, PageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        _first_page = new ((PageNode*)map) PageNode(nullptr, map);
        _current_page = _first_page;
        std::cout << "sizeof(PageNode): " << sizeof(PageNode) << std::endl;
        map = (void*)((char*)map + sizeof(PageNode));
        _current_page->first_block_data = new ((BlockData *)map) BlockData(nullptr, 
                                                             nullptr, 
                                                             map, 
                                                             PageSize - sizeof(BlockData) - sizeof(PageNode), 
                                                             false);
        _current_page->count_of_blocks = 1;
    }

    void destroy()
    {
        PageNode* temp = _first_page;

        for (size_t i = 0; i < _count_of_pages; i++)
        {
            PageNode* second_temp = temp;
            temp = temp->next;
            munmap(second_temp->memory, PageSize);
        }
        _first_page = nullptr;
        _current_page = nullptr;
        _inited = false;
    }

    void* alloc(size_t size)
    {
        std::cout << "sizeof(BlockData): " << sizeof(BlockData) << std::endl;

        for (size_t i = 0; i < _count_of_pages; i++)
        {
            void* ptr;

            BlockData * temp = _current_page->first_block_data;
            for (size_t i = 0; i < _current_page->count_of_blocks; i++)
            {
                std::cout << "i: " << i << std::endl;

                BlockData* temp_block_data;

                if (temp->with_value == false &&
                    temp->size >= size)
                {
                    _current_page->count_of_blocks++;
                    ptr = (char*)temp->memory + sizeof(BlockData);

                    temp->with_value = true;
                    temp_block_data = new ((char*)ptr + size) BlockData(temp->next,
                                                      (BlockData*)temp->memory,
                                                      (void*)((char*)ptr + size),
                                                      temp->size - ((char*)ptr + size - (char*)temp->memory),
                                                      false);

                    temp->size = size;
                    temp->next = temp_block_data;
                    return ptr;
                }

                temp = temp->next;
            }
            _count_of_pages++;
            void* map = mmap(NULL, PageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
            _current_page->next = new ((PageNode*)map) PageNode(nullptr, map);
            _current_page = _current_page->next;
            map = (void*)((char*)map + sizeof(PageNode));
            _current_page->first_block_data = new ((BlockData *)map) BlockData(nullptr, 
                                                             nullptr, 
                                                             map, 
                                                             PageSize - sizeof(BlockData) - sizeof(PageNode), 
                                                             false);
            _current_page->count_of_blocks = 1;
            return alloc(size);
        }

        return nullptr;
    }

    void free(void* p)
    {
        std::cout << "FREE" << std::endl;
        PageNode* temp = _first_page;

        for (size_t i = 0; i < _count_of_pages; i++)
        {
            BlockData* temp_block = _current_page->first_block_data;
            
            for (size_t i = 0; i < _current_page->count_of_blocks; i++)
            {
                void* p_temp = (char*)temp_block + sizeof(BlockData);
                if (p_temp == p)
                {
                    std::cout << "Блок совпадает" << std::endl;
                    BlockData* found_block = (BlockData*)((char*)p_temp - sizeof(BlockData));
                    found_block->with_value = false;
                    if (found_block->next != nullptr && 
                        found_block->next->with_value == false)
                    {
                        temp->count_of_blocks--;
                        found_block->size += found_block->next->size + sizeof(BlockData);
                        if (found_block->next->next != nullptr)
                        {
                            found_block->next->next->prev = found_block;
                        }
                        found_block->next = found_block->next->next;
                    }
                    if (found_block->prev != nullptr &&
                        found_block->prev->with_value == false)
                    {
                        temp->count_of_blocks--;
                        found_block->prev->size += found_block->size + sizeof(BlockData);
                        found_block->prev->next = found_block->next;
                    }
                    break;
                }
                
                temp_block = temp_block->next;
            }
            temp = temp->next;
        }
        
        // std::cout << "FREE" << std::endl;
        // BlockData* temp_block = (BlockData*)((char*)p - sizeof(BlockData));
        // temp->with_value = false;
        // if (temp->next != nullptr && 
        //     temp->next->with_value == false)
        // {
        //     // Вынести количество блоков под управлении страницы
        //     _current_page->count_of_blocks--;
        //     temp->size += temp->next->size + sizeof(BlockData);
        //     if (temp->next->next != nullptr)
        //     {
        //         temp->next->next->prev = temp;
        //     }
        //     temp->next = temp->next->next;
        // }
        // if (temp->prev != nullptr &&
        //     temp->prev->with_value == false)
        // {
        //     _current_page->count_of_blocks--;
        //     temp->prev->size += temp->size + sizeof(BlockData);
        //     temp->prev->next = temp->next;
        // }

        // std::cout << "Value of block: " << *((int*)((char*)temp->memory + sizeof(BlockData))) << std::endl;
        // std::cout << std::endl;
    }

    //TEST METHODS
    void show_data()
    {
        BlockData* temp = _current_page->first_block_data;
        for (size_t i = 0; i < _current_page->count_of_blocks; i++)
        {
            std::cout << "Count of block data: " << i + 1 << std::endl;
            std::cout << "Size of block info: " << temp->size << std::endl;
            std::cout << "Value of block: " << *((int*)((char*)temp->memory + sizeof(BlockData))) << std::endl;
            std::cout << "With value: " << temp->with_value << std::endl;
            std::cout << std::endl;

            temp = temp->next;
        }
    }


private:
    const static size_t PageSize = 512;
    bool _inited;
    // size_t _count_of_blocks;
    size_t _count_of_pages;
    // Temp переменная для ссылки на первую страницу
    // void* map;
    
    PageNode* _current_page;
    PageNode* _first_page;
};