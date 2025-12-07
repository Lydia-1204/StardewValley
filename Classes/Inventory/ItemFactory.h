#pragma once
#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "Item.h"
#include <functional>
#include <unordered_map>

class ItemFactory
{
public:
    // 获取单例实例
    static ItemFactory *getInstance();

    // 创建指定类型的物品
    Item *createItem(Item::ItemType type);

    // 预加载物品到对象池
    void preloadItems(Item::ItemType type, int count);

    // 清理工厂资源
    void cleanup();

private:
    ItemFactory();
    ~ItemFactory();

    // 禁用拷贝构造和赋值
    ItemFactory(const ItemFactory &) = delete;
    ItemFactory &operator=(const ItemFactory &) = delete;

    static ItemFactory *_instance;

    // 注册所有物品类型的创建函数
    void registerCreators();

    // 创建函数映射表
    std::unordered_map<Item::ItemType, std::function<Item *()>> _creators;

    // 对象池支持
    std::unordered_map<Item::ItemType, std::vector<Item *>> _itemPools;
};

#endif // ITEMFACTORY_H