require 'lubyk'
lk.Doc.make {
  sources = {
    'modules/core/lib',
    'modules/lk/lib',
    'modules/test/lib',
    'modules/dub/lib',
  },
  target = 'gh-pages',
  format = 'html',
  header = [[ <a href='http://lubyk.org'><img alt='lubyk logo' src='img/lubyk.png'/> <h1>Lubyk documentation</h1></a> ]],
}

os.execute('cp gh-pages/lk.html gh-pages/index.html')

