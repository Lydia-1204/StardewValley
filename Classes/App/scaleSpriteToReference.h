#pragma once
#include "cocos2d.h"

using namespace cocos2d;

/**
 * @brief 将一张图片缩放为另一张图片大小的某个倍数
 * @param spriteToScale 要缩放的图片精灵
 * @param referenceSprite 参考图片精灵
 * @param multiple 缩放倍数
 */
void scaleSpriteToReference(Sprite* spriteToScale, Sprite* referenceSprite, float multiple)
{
    if (!spriteToScale || !referenceSprite) {
        throw std::invalid_argument("One or both sprites are null!");
    }

    // 获取参考精灵的宽高
    const Size referenceSize = referenceSprite->getContentSize();

    // 获取需要缩放精灵的宽高
    const Size spriteSize = spriteToScale->getContentSize();

    if (spriteSize.width == 0 || spriteSize.height == 0) {
        throw std::runtime_error("Invalid sprite size! Width or height is zero.");
    }

    // 计算缩放比例
    float scaleX = (referenceSize.width * multiple) / spriteSize.width;
    float scaleY = (referenceSize.height * multiple) / spriteSize.height;

    // 设置精灵缩放
    spriteToScale->setScale(scaleX, scaleY);
}
