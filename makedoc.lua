require 'lubyk'
lk.Doc.make {
  sources = {
    'modules/core/lib',
    'modules/lk/lib',
    'modules/test/lib',
    'modules/dub/lib',
    'modules/four/lib',
    {'modules/four/doc/examples', prepend='tutorial/four'},
  },
  copy   = {
    'modules/lk/doc',
    'modules/four/doc',
  },
  target = 'gh-pages',
  format = 'html',
  header = [[ <a href='http://lubyk.org'><img alt='lubyk logo' src='img/lubyk.png'/></a> <h1><a href='http://doc.lubyk.org'>Lubyk documentation</a></h1></a> ]],
}

os.execute('cp gh-pages/lk.html gh-pages/index.html')

