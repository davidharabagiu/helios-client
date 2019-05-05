#ifndef QTMODELS_H
#define QTMODELS_H

class QtModels
{
public:
    /**
     * @brief Register types in QML domain
     * @param uri - Module uri
     */
    static void registerQmlTypes(const char* uri);
};

#endif  // QTMODELS_H
