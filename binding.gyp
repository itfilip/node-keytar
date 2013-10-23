{
  'targets': [
    {
      'target_name': 'keytar',
      'sources': [
        'src/main.cc',
        'src/keytar.h',
      ],
      'conditions': [
        ['OS=="mac"', {
          'sources': [
            'src/keytar_mac.cc',
          ],
        }],
        ['OS=="win"', {
          'sources': [
            'src/keytar_win.cc',
          ],
          'msvs_disabled_warnings': [
            4267,  # conversion from 'size_t' to 'int', possible loss of data
            4530,  # C++ exception handler used, but unwind semantics are not enabled
            4506,  # no definition for inline function
          ],
        }],
      ],
    }
  ]
}