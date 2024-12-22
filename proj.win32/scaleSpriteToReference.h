#pragma once
#include "cocos2d.h"

using namespace cocos2d;

/**
 * @brief ��һ��ͼƬ����Ϊ��һ��ͼƬ��С��ĳ������
 * @param spriteToScale Ҫ���ŵ�ͼƬ����
 * @param referenceSprite �ο�ͼƬ����
 * @param multiple ���ű���
 */
void scaleSpriteToReference(Sprite* spriteToScale, Sprite* referenceSprite, float multiple)
{
    if (!spriteToScale || !referenceSprite) {
        throw std::invalid_argument("One or both sprites are null!");
    }

    // ��ȡ�ο�����Ŀ��
    const Size referenceSize = referenceSprite->getContentSize();

    // ��ȡ��Ҫ���ž���Ŀ��
    const Size spriteSize = spriteToScale->getContentSize();

    if (spriteSize.width == 0 || spriteSize.height == 0) {
        throw std::runtime_error("Invalid sprite size! Width or height is zero.");
    }

    // �������ű���
    float scaleX = (referenceSize.width * multiple) / spriteSize.width;
    float scaleY = (referenceSize.height * multiple) / spriteSize.height;

    // ���þ�������
    spriteToScale->setScale(scaleX, scaleY);
}