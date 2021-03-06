#include "common.h"

// Includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace v8;
using namespace node;

#include <iostream>
using namespace std;

namespace glu {

    class GLU : public ObjectWrap {
    public:
    private:
        void *ptr;
    };

    // forward declarations
    enum GLUObjectType {
        GLUOBJECT_TYPE_NURBSRENDERER,
        GLUOBJECT_TYPE_QUADRICS,
        GLUOBJECT_TYPE_TESS,
    };
    void registerGLUObj(GLUObjectType type, void *obj);
    void unregisterGLUObj(void *obj);

    inline void *getImageData(Local<Value> arg) {
        void *pixels = NULL;
        if (!arg->IsNull()) {
            Local<Object> obj = Local<Object>::Cast(arg);
            if (!obj->IsObject())
                ThrowException(JS_STR("Bad texture argument"));

            pixels = obj->GetIndexedPropertiesExternalArrayData();
        }
        return pixels;
    }
    inline Local<Object> obj2wrap(const char *name, void *ptr) {
        Handle<FunctionTemplate> point_templ = FunctionTemplate::New();
        point_templ->SetClassName(String::New(name));
        Handle<ObjectTemplate> point_inst = point_templ->InstanceTemplate();
        point_inst->SetInternalFieldCount(1);
        Handle<Function> point_ctor = point_templ->GetFunction();
        Local<Object> obj = point_ctor->NewInstance();
        obj->SetInternalField(0, External::New(ptr));
        return obj;
    }
    inline void *wrap2obj(const char *name, Local<Value> arg) {
        if (arg->IsObject()) {
            Local<Object> obj = arg->ToObject();
            Local<External> wrap = Local<External>::Cast(obj->GetInternalField(0));
            void *ptr = wrap->Value();
#if LOGGING
            cout << ptr << "\n";
#endif
            return ptr;
        } else {
            ThrowException(JS_STR("Not an object"));
            return NULL;
        }
    }


    /* GLFW initialization, termination and version querying */

    JS_METHOD(Init) {
        HandleScope scope;
        return scope.Close(Undefined());
    }

    JS_METHOD(checkExtension) {
        HandleScope scope;
        String::Utf8Value name(args[0]);
        String::Utf8Value string(args[1]);
        return scope.Close(Boolean::New(gluCheckExtension((GLubyte *)*name, (GLubyte *)*string)));
    }

    JS_METHOD(getString) {
        HandleScope scope;
        GLenum name = args[0]->Int32Value();
        return scope.Close(String::New((char *)gluGetString(name)));
    }

    JS_METHOD(createNurbsRenderer) {
        HandleScope scope;
        GLUnurbs *nurbsRenderer = gluNewNurbsRenderer();
#ifdef LOGGING
        cout << "create nurbs renderer " << nurbsRenderer << endl;
#endif
        registerGLUObj(GLUOBJECT_TYPE_NURBSRENDERER, nurbsRenderer);
        return scope.Close(obj2wrap("GLUnurbs", nurbsRenderer));
    }

    JS_METHOD(createQuadric) {
        HandleScope scope;
        GLUquadric *quadrics = gluNewQuadric();
#ifdef LOGGING
        cout << "create quadrics  " << quadrics << endl;
#endif
        registerGLUObj(GLUOBJECT_TYPE_QUADRICS, quadrics);
        return scope.Close(obj2wrap("GLUquadric", quadrics));
    }

    JS_METHOD(createTess) {
        HandleScope scope;
        GLUtesselator *tess = gluNewTess();
#ifdef LOGGING
        cout << "create tess  " << tess << endl;
#endif
        registerGLUObj(GLUOBJECT_TYPE_TESS, tess);
        return scope.Close(obj2wrap("GLUtesselator", tess));
    }

    JS_METHOD(beginCurve) {
        HandleScope scope;
        GLUnurbs *ptr = (GLUnurbs *)wrap2obj("GLUnurbs", args[0]);
        gluBeginCurve(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(beginPolygon) {
        HandleScope scope;
        GLUtesselator *ptr = (GLUtesselator *)wrap2obj("GLUtesselator", args[0]);
        gluBeginPolygon(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(beginSurface) {
        HandleScope scope;
        GLUnurbs *ptr = (GLUnurbs *)wrap2obj("GLUnurbs", args[0]);
        gluBeginSurface(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(beginTrim) {
        HandleScope scope;
        GLUnurbs *ptr = (GLUnurbs *)wrap2obj("GLUnurbs", args[0]);
        gluBeginTrim(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(endCurve) {
        HandleScope scope;
        GLUnurbs *ptr = (GLUnurbs *)wrap2obj("GLUnurbs", args[0]);
        gluEndCurve(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(endPolygon) {
        HandleScope scope;
        GLUtesselator *ptr = (GLUtesselator *)wrap2obj("GLUtesselator", args[0]);
        gluEndPolygon(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(endSurface) {
        HandleScope scope;
        GLUnurbs *ptr = (GLUnurbs *)wrap2obj("GLUnurbs", args[0]);
        gluEndSurface(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(endTrim) {
        HandleScope scope;
        GLUnurbs *ptr = (GLUnurbs *)wrap2obj("GLUnurbs", args[0]);
        gluEndTrim(ptr);
        return scope.Close(Undefined());
    }

    JS_METHOD(cylinder) {
        HandleScope scope;
        GLUquadric *quad = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLdouble base = (GLdouble) args[1]->NumberValue();
        GLdouble top = (GLdouble) args[2]->NumberValue();
        GLdouble height = (GLdouble) args[3]->NumberValue();
        GLint slices = (GLint) args[4]->Int32Value();
        GLint loops = (GLint) args[5]->Int32Value();
        gluCylinder(quad, base, top, height, slices, loops);
        return scope.Close(Undefined());
    }

    JS_METHOD(disk) {
        HandleScope scope;
        GLUquadric *quad = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLdouble inner = (GLdouble) args[1]->NumberValue();
        GLdouble outer = (GLdouble) args[2]->NumberValue();
        GLint slices = (GLint) args[3]->Int32Value();
        GLint loops = (GLint) args[4]->Int32Value();
        gluDisk(quad, inner, outer, slices, loops);
        return scope.Close(Undefined());
    }

    JS_METHOD(partialDisk) {
        HandleScope scope;
        GLUquadric *quad = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLdouble inner = (GLdouble) args[1]->NumberValue();
        GLdouble outer = (GLdouble) args[2]->NumberValue();
        GLint slices = (GLint) args[3]->Int32Value();
        GLint loops = (GLint) args[4]->Int32Value();
        GLdouble start = (GLdouble) args[5]->NumberValue();
        GLdouble sweep = (GLdouble) args[6]->NumberValue();
        gluPartialDisk(quad, inner, outer, slices, loops, start, sweep);
        return scope.Close(Undefined());
    }

    JS_METHOD(sphere) {
        HandleScope scope;
        GLUquadric *quad = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLdouble radius = (GLdouble) args[1]->NumberValue();
        GLint slices = (GLint) args[2]->Int32Value();
        GLint stacks = (GLint) args[3]->Int32Value();
        gluSphere(quad, radius, slices, stacks);
        return scope.Close(Undefined());
    }

    JS_METHOD(perspective) {
        HandleScope scope;
        GLdouble fovy = (GLdouble) args[0]->NumberValue();
        GLdouble aspect = (GLdouble) args[1]->NumberValue();
        GLdouble zNear = (GLdouble) args[2]->NumberValue();
        GLdouble zFar = (GLdouble) args[3]->NumberValue();
        gluPerspective(fovy, aspect, zNear, zFar);
        return scope.Close(Undefined());
    }

    JS_METHOD(ortho2D) {
        HandleScope scope;
        GLdouble left = (GLdouble) args[0]->NumberValue();
        GLdouble right = (GLdouble) args[1]->NumberValue();
        GLdouble bottom = (GLdouble) args[2]->NumberValue();
        GLdouble top = (GLdouble) args[3]->NumberValue();
        gluOrtho2D(left, right, bottom, top);
        return scope.Close(Undefined());
    }

    JS_METHOD(lookAt) {
        HandleScope scope;
        GLdouble eyeX = (GLdouble) args[0]->NumberValue();
        GLdouble eyeY = (GLdouble) args[1]->NumberValue();
        GLdouble eyeZ = (GLdouble) args[2]->NumberValue();
        GLdouble centerX = (GLdouble) args[3]->NumberValue();
        GLdouble centerY = (GLdouble) args[4]->NumberValue();
        GLdouble centerZ = (GLdouble) args[5]->NumberValue();
        GLdouble upX = (GLdouble) args[6]->NumberValue();
        GLdouble upY = (GLdouble) args[7]->NumberValue();
        GLdouble upZ = (GLdouble) args[8]->NumberValue();
        gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
        return scope.Close(Undefined());
    }

    JS_METHOD(nextContour) {
        HandleScope scope;
        GLUtesselator *tess = (GLUtesselator *)wrap2obj("GLUtesselator", args[0]);
        GLenum type = (GLenum) args[1]->Int32Value();
        gluNextContour(tess, type);
        return scope.Close(Undefined());
    }

    JS_METHOD(build2DMipmaps) {
        HandleScope scope;

        int target = args[0]->Int32Value();
        int internalformat = args[1]->Int32Value();
        int width = args[2]->Int32Value();
        int height = args[3]->Int32Value();
        int format = args[4]->Int32Value();
        int type = args[5]->Int32Value();
        void *pixels = getImageData(args[6]);

        gluBuild2DMipmaps(target, internalformat, width, height, format, type, pixels);

        return scope.Close(Undefined());
    }

    JS_METHOD(quadricNormals) {
        HandleScope scope;
        GLUquadric *quadric = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLenum type = (GLenum) args[1]->Int32Value();
        gluQuadricNormals(quadric, type);
        return scope.Close(Undefined());
    }

    JS_METHOD(quadricTexture) {
        HandleScope scope;
        GLUquadric *quadric = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLboolean flag = args[1]->BooleanValue();
        gluQuadricTexture(quadric, flag);
        return scope.Close(Undefined());
    }

    JS_METHOD(quadricOrientation) {
        HandleScope scope;
        GLUquadric *quadric = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLenum orientation = args[1]->Int32Value();
        gluQuadricOrientation(quadric, orientation);
        return scope.Close(Undefined());
    }

    JS_METHOD(quadricDrawStyle) {
        HandleScope scope;
        GLUquadric *quadric = (GLUquadric *)wrap2obj("GLUquadric", args[0]);
        GLenum draw = args[1]->Int32Value();
        gluQuadricDrawStyle(quadric, draw);
        return scope.Close(Undefined());
    }

    JS_METHOD(tessNormal) {
        HandleScope scope;
        GLUtesselator *tess = (GLUtesselator *)wrap2obj("GLUtesselator", args[0]);
        GLdouble valueX = args[1]->NumberValue();
        GLdouble valueY = args[2]->NumberValue();
        GLdouble valueZ = args[3]->NumberValue();
        gluTessNormal(tess, valueX, valueY, valueZ);
        return scope.Close(Undefined());
    }

    struct GLUObj {
        GLUObjectType type;
        void *obj;
        GLUObj(GLUObjectType type, void *obj) {
            this->type = type;
            this->obj = obj;
        }
    };

    vector<GLUObj *> gluobjs;
    static bool atExit = false;

    void registerGLUObj(GLUObjectType type, void *obj) {
        gluobjs.push_back(new GLUObj(type, obj));
    }


    void unregisterGLUObj(void *obj) {
        if (atExit) return;

        vector<GLUObj *>::iterator it = gluobjs.begin();
        while (gluobjs.size() && it != gluobjs.end()) {
            GLUObj *gluobj = *it;
            if (gluobj->obj == obj) {
                delete gluobj;
                gluobjs.erase(it);
                break;
            }
            ++it;
        }
    }

    void AtExit() {
        atExit = true;
        //glFinish();

        vector<GLUObj *>::iterator it;

#ifdef LOGGING
        cout << "GLU AtExit() called" << endl;
        cout << "  # objects allocated: " << gluobjs.size() << endl;
        it = gluobjs.begin();
        while (gluobjs.size() && it != gluobjs.end()) {
            GLUObj *obj = *it;
            cout << "[";
            switch (obj->type) {
            case GLUOBJECT_TYPE_NURBSRENDERER: cout << "nurbsrenderer"; break;
            case GLUOBJECT_TYPE_TESS: cout << "tess"; break;
            case GLUOBJECT_TYPE_QUADRICS: cout << "quadrics"; break;
            };
            cout << ": " << obj->obj << "] ";
            ++it;
        }
        cout << endl;
#endif

        it = gluobjs.begin();
        while (gluobjs.size() && it != gluobjs.end()) {
            GLUObj *gluobj = *it;
            void *obj = gluobj->obj;

            switch (gluobj->type) {
            case GLUOBJECT_TYPE_NURBSRENDERER:
#ifdef LOGGING
                cout << "  Destroying GL nurbs renderer " << obj << endl;
#endif
                gluDeleteNurbsRenderer((GLUnurbs *)obj);
                break;
            case GLUOBJECT_TYPE_TESS:
#ifdef LOGGING
                cout << "  Destroying GL tess " << obj << endl;
#endif
                gluDeleteTess((GLUtesselator *)obj);
                break;
            case GLUOBJECT_TYPE_QUADRICS:
#ifdef LOGGING
                cout << "  Destroying GL quadrics " << obj << endl;
#endif
                gluDeleteQuadric((GLUquadric *)obj);
                break;
            default:
#ifdef LOGGING
                cout << "  Unknown object " << obj << endl;
#endif
                break;
            }
            delete gluobj;
            ++it;
        }

        gluobjs.clear();
    }

} // namespace glfw

///////////////////////////////////////////////////////////////////////////////
//
// bindings
//
///////////////////////////////////////////////////////////////////////////////
#define JS_GLU_CONSTANT(name) target->Set(JS_STR( #name ), JS_INT(GLU_ ## name))
#define JS_GLU_SET_METHOD(name) NODE_SET_METHOD(target, #name , glu::name);

extern "C" {
    void init(Handle<Object> target) {
        atexit(glu::AtExit);

        HandleScope scope;

        /* GLFW initialization, termination and version querying */
        JS_GLU_SET_METHOD(Init);

        JS_GLU_SET_METHOD(checkExtension);
        JS_GLU_SET_METHOD(getString);

        JS_GLU_SET_METHOD(createNurbsRenderer);
        JS_GLU_SET_METHOD(createQuadric);
        JS_GLU_SET_METHOD(createTess);

        JS_GLU_SET_METHOD(beginCurve);
        JS_GLU_SET_METHOD(beginPolygon);
        JS_GLU_SET_METHOD(beginSurface);
        JS_GLU_SET_METHOD(beginTrim);

        JS_GLU_SET_METHOD(endCurve);
        JS_GLU_SET_METHOD(endPolygon);
        JS_GLU_SET_METHOD(endSurface);
        JS_GLU_SET_METHOD(endTrim);

        JS_GLU_SET_METHOD(cylinder);
        JS_GLU_SET_METHOD(disk);
        JS_GLU_SET_METHOD(partialDisk);
        JS_GLU_SET_METHOD(sphere);

        JS_GLU_SET_METHOD(ortho2D);
        JS_GLU_SET_METHOD(perspective);
        JS_GLU_SET_METHOD(lookAt);
        JS_GLU_SET_METHOD(build2DMipmaps);

        JS_GLU_SET_METHOD(quadricNormals);
        JS_GLU_SET_METHOD(quadricTexture);
        JS_GLU_SET_METHOD(quadricOrientation);
        JS_GLU_SET_METHOD(quadricDrawStyle);

        JS_GLU_SET_METHOD(tessNormal);

        /*************************************************************************
         * GLFW version
         *************************************************************************/

        JS_GLU_CONSTANT(EXT_object_space_tess);
        JS_GLU_CONSTANT(EXT_nurbs_tessellator);
        JS_GLU_CONSTANT(FALSE);
        JS_GLU_CONSTANT(TRUE);
        JS_GLU_CONSTANT(VERSION_1_1);
        JS_GLU_CONSTANT(VERSION_1_2);
        JS_GLU_CONSTANT(VERSION_1_3);
        JS_GLU_CONSTANT(VERSION);
        JS_GLU_CONSTANT(EXTENSIONS);

        /* ErrorCode */
        JS_GLU_CONSTANT(INVALID_ENUM);
        JS_GLU_CONSTANT(INVALID_VALUE);
        JS_GLU_CONSTANT(OUT_OF_MEMORY);
        JS_GLU_CONSTANT(INCOMPATIBLE_GL_VERSION);
        JS_GLU_CONSTANT(INVALID_OPERATION);
        /* NurbsDisplay */
        JS_GLU_CONSTANT(OUTLINE_POLYGON);
        JS_GLU_CONSTANT(OUTLINE_PATCH);
        /* NurbsCallback */
        JS_GLU_CONSTANT(NURBS_ERROR);
        JS_GLU_CONSTANT(ERROR);
        JS_GLU_CONSTANT(NURBS_BEGIN);
        JS_GLU_CONSTANT(NURBS_BEGIN_EXT);
        JS_GLU_CONSTANT(NURBS_VERTEX);
        JS_GLU_CONSTANT(NURBS_VERTEX_EXT);
        JS_GLU_CONSTANT(NURBS_NORMAL);
        JS_GLU_CONSTANT(NURBS_NORMAL_EXT);
        JS_GLU_CONSTANT(NURBS_COLOR);
        JS_GLU_CONSTANT(NURBS_COLOR_EXT);
        JS_GLU_CONSTANT(NURBS_TEXTURE_COORD);
        JS_GLU_CONSTANT(NURBS_TEX_COORD_EXT);
        JS_GLU_CONSTANT(NURBS_END);
        JS_GLU_CONSTANT(NURBS_END_EXT);
        JS_GLU_CONSTANT(NURBS_BEGIN_DATA);
        JS_GLU_CONSTANT(NURBS_BEGIN_DATA_EXT);
        JS_GLU_CONSTANT(NURBS_VERTEX_DATA);
        JS_GLU_CONSTANT(NURBS_VERTEX_DATA_EXT);
        JS_GLU_CONSTANT(NURBS_NORMAL_DATA);
        JS_GLU_CONSTANT(NURBS_NORMAL_DATA_EXT);
        JS_GLU_CONSTANT(NURBS_COLOR_DATA);
        JS_GLU_CONSTANT(NURBS_COLOR_DATA_EXT);
        JS_GLU_CONSTANT(NURBS_TEXTURE_COORD_DATA);
        JS_GLU_CONSTANT(NURBS_TEX_COORD_DATA_EXT);
        JS_GLU_CONSTANT(NURBS_END_DATA);
        JS_GLU_CONSTANT(NURBS_END_DATA_EXT);

        /* NurbsError */
        JS_GLU_CONSTANT(NURBS_ERROR1);
        JS_GLU_CONSTANT(NURBS_ERROR2);
        JS_GLU_CONSTANT(NURBS_ERROR3);
        JS_GLU_CONSTANT(NURBS_ERROR4);
        JS_GLU_CONSTANT(NURBS_ERROR5);
        JS_GLU_CONSTANT(NURBS_ERROR6);
        JS_GLU_CONSTANT(NURBS_ERROR7);
        JS_GLU_CONSTANT(NURBS_ERROR8);
        JS_GLU_CONSTANT(NURBS_ERROR9);
        JS_GLU_CONSTANT(NURBS_ERROR10);
        JS_GLU_CONSTANT(NURBS_ERROR11);
        JS_GLU_CONSTANT(NURBS_ERROR12);
        JS_GLU_CONSTANT(NURBS_ERROR13);
        JS_GLU_CONSTANT(NURBS_ERROR14);
        JS_GLU_CONSTANT(NURBS_ERROR15);
        JS_GLU_CONSTANT(NURBS_ERROR16);
        JS_GLU_CONSTANT(NURBS_ERROR17);
        JS_GLU_CONSTANT(NURBS_ERROR18);
        JS_GLU_CONSTANT(NURBS_ERROR19);
        JS_GLU_CONSTANT(NURBS_ERROR20);
        JS_GLU_CONSTANT(NURBS_ERROR21);
        JS_GLU_CONSTANT(NURBS_ERROR22);
        JS_GLU_CONSTANT(NURBS_ERROR23);
        JS_GLU_CONSTANT(NURBS_ERROR24);
        JS_GLU_CONSTANT(NURBS_ERROR25);
        JS_GLU_CONSTANT(NURBS_ERROR26);
        JS_GLU_CONSTANT(NURBS_ERROR27);
        JS_GLU_CONSTANT(NURBS_ERROR28);
        JS_GLU_CONSTANT(NURBS_ERROR29);
        JS_GLU_CONSTANT(NURBS_ERROR30);
        JS_GLU_CONSTANT(NURBS_ERROR31);
        JS_GLU_CONSTANT(NURBS_ERROR32);
        JS_GLU_CONSTANT(NURBS_ERROR33);
        JS_GLU_CONSTANT(NURBS_ERROR34);
        JS_GLU_CONSTANT(NURBS_ERROR35);
        JS_GLU_CONSTANT(NURBS_ERROR36);
        JS_GLU_CONSTANT(NURBS_ERROR37);

        /* NurbsProperty */
        JS_GLU_CONSTANT(AUTO_LOAD_MATRIX);
        JS_GLU_CONSTANT(CULLING);
        JS_GLU_CONSTANT(SAMPLING_TOLERANCE);
        JS_GLU_CONSTANT(DISPLAY_MODE);
        JS_GLU_CONSTANT(PARAMETRIC_TOLERANCE);
        JS_GLU_CONSTANT(SAMPLING_METHOD);
        JS_GLU_CONSTANT(U_STEP);
        JS_GLU_CONSTANT(V_STEP);
        JS_GLU_CONSTANT(NURBS_MODE);
        JS_GLU_CONSTANT(NURBS_MODE_EXT);
        JS_GLU_CONSTANT(NURBS_TESSELLATOR);
        JS_GLU_CONSTANT(NURBS_TESSELLATOR_EXT);
        JS_GLU_CONSTANT(NURBS_RENDERER);
        JS_GLU_CONSTANT(NURBS_RENDERER_EXT);

        /* NurbsSampling */
        JS_GLU_CONSTANT(OBJECT_PARAMETRIC_ERROR);
        JS_GLU_CONSTANT(OBJECT_PARAMETRIC_ERROR_EXT);
        JS_GLU_CONSTANT(OBJECT_PATH_LENGTH);
        JS_GLU_CONSTANT(OBJECT_PATH_LENGTH_EXT);
        JS_GLU_CONSTANT(PATH_LENGTH);
        JS_GLU_CONSTANT(PARAMETRIC_ERROR);
        JS_GLU_CONSTANT(DOMAIN_DISTANCE);

        /* NurbsTrim */
        JS_GLU_CONSTANT(MAP1_TRIM_2);
        JS_GLU_CONSTANT(MAP1_TRIM_3);

        /* QuadricDrawStyle */
        JS_GLU_CONSTANT(POINT);
        JS_GLU_CONSTANT(LINE);
        JS_GLU_CONSTANT(FILL);
        JS_GLU_CONSTANT(SILHOUETTE);

        /* QuadricCallback */
        /*      GLU_ERROR */

        /* QuadricNormal */
        JS_GLU_CONSTANT(SMOOTH);
        JS_GLU_CONSTANT(FLAT);
        JS_GLU_CONSTANT(NONE);

        /* QuadricOrientation */
        JS_GLU_CONSTANT(OUTSIDE);
        JS_GLU_CONSTANT(INSIDE);

        /* TessCallback */
        JS_GLU_CONSTANT(TESS_BEGIN);
        JS_GLU_CONSTANT(BEGIN);
        JS_GLU_CONSTANT(TESS_VERTEX);
        JS_GLU_CONSTANT(VERTEX);
        JS_GLU_CONSTANT(TESS_END);
        JS_GLU_CONSTANT(END);
        JS_GLU_CONSTANT(TESS_ERROR);
        JS_GLU_CONSTANT(TESS_EDGE_FLAG);
        JS_GLU_CONSTANT(EDGE_FLAG);
        JS_GLU_CONSTANT(TESS_COMBINE);
        JS_GLU_CONSTANT(TESS_BEGIN_DATA);
        JS_GLU_CONSTANT(TESS_VERTEX_DATA);
        JS_GLU_CONSTANT(TESS_END_DATA);
        JS_GLU_CONSTANT(TESS_ERROR_DATA);
        JS_GLU_CONSTANT(TESS_EDGE_FLAG_DATA);
        JS_GLU_CONSTANT(TESS_COMBINE_DATA);

        /* TessContour */
        JS_GLU_CONSTANT(CW);
        JS_GLU_CONSTANT(CCW);
        JS_GLU_CONSTANT(INTERIOR);
        JS_GLU_CONSTANT(EXTERIOR);
        JS_GLU_CONSTANT(UNKNOWN);

        /* TessProperty */
        JS_GLU_CONSTANT(TESS_WINDING_RULE);
        JS_GLU_CONSTANT(TESS_BOUNDARY_ONLY);
        JS_GLU_CONSTANT(TESS_TOLERANCE);

        /* TessError */
        JS_GLU_CONSTANT(TESS_ERROR1);
        JS_GLU_CONSTANT(TESS_ERROR2);
        JS_GLU_CONSTANT(TESS_ERROR3);
        JS_GLU_CONSTANT(TESS_ERROR4);
        JS_GLU_CONSTANT(TESS_ERROR5);
        JS_GLU_CONSTANT(TESS_ERROR6);
        JS_GLU_CONSTANT(TESS_ERROR7);
        JS_GLU_CONSTANT(TESS_ERROR8);
        JS_GLU_CONSTANT(TESS_MISSING_BEGIN_POLYGON);
        JS_GLU_CONSTANT(TESS_MISSING_BEGIN_CONTOUR);
        JS_GLU_CONSTANT(TESS_MISSING_END_POLYGON);
        JS_GLU_CONSTANT(TESS_MISSING_END_CONTOUR);
        JS_GLU_CONSTANT(TESS_COORD_TOO_LARGE);
        JS_GLU_CONSTANT(TESS_NEED_COMBINE_CALLBACK);

        /* TessWinding */
        JS_GLU_CONSTANT(TESS_WINDING_ODD);
        JS_GLU_CONSTANT(TESS_WINDING_NONZERO);
        JS_GLU_CONSTANT(TESS_WINDING_POSITIVE);
        JS_GLU_CONSTANT(TESS_WINDING_NEGATIVE);
        JS_GLU_CONSTANT(TESS_WINDING_ABS_GEQ_TWO);

#define GLU_TESS_MAX_COORD 1.0e150

        JS_GLU_CONSTANT(TESS_MAX_COORD);
    }

    NODE_MODULE(glu, init)
}

