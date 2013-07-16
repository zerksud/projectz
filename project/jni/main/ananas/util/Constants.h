/*
 * Constants.h
 *
 *  Created on: Apr 3, 2013
 *      Author: zerksud
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace ann {
namespace util {

class Constants {
public:
    static const char* kDebugValueNotification;

    static const char* kConfigLocation;
    static const char* kMeshesLocation;
    static const char* kShadersLocation;
    static const char* kTexturesLocation;

    static const char* kActivityOnCreate;
    static const char* kActivityOnResume;
    static const char* kActivityOnPause;
    static const char* kActivityOnStop;

    static const char* kSurfaceSizeChanged;
    static const char* kSurfaceDestroyed;

    static const char* kMeshLoaded;

    static const char* kUIDebugToggleWireframe;

private:
    Constants();
    Constants(Constants& other);
    virtual ~Constants();
    Constants& operator=(const Constants& other);
};

} /* namespace util */
} /* namespace ann */
#endif /* CONSTANTS_H_ */
