{
  'variables': {
    'platform': '<(OS)',
  },
  'conditions': [
    # Replace gyp platform with node platform, blech
    ['platform == "mac"', {'variables': {'platform': 'darwin'}}],
    ['platform == "win"', {'variables': {'platform': 'win32'}}],
  ],
  'targets': [
    {
      #'target_name': 'glu-<(platform)-<(target_arch)',
      'target_name': 'glu',
      'defines': [
        'VERSION=0.0.1'
      ],
      'sources': [ 'src/glu.cc' ],
      'include_dirs': [
        './deps/include',
      ],
      'library_dirs': [
        './deps/<(platform)',
      ],
      'conditions': [
        ['OS=="linux"', {'libraries': ['-lGLU']}],
        ['OS=="mac"', {'libraries': [ '-lGLU', '-framework OpenGL']}],
        ['OS=="win"', {
          'libraries': [
            'glu32.lib', 
            'opengl32.lib'
            ],
          'defines' : [
            'WIN32_LEAN_AND_MEAN',
            'VC_EXTRALEAN'
          ]
          },
        ],
      ],
    }
  ]
}
