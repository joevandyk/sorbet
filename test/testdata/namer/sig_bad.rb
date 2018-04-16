# @typed
class T1; end
class T2; end

class A
  sig(
    a: unsupported, # error: Unknown type syntax
    b: T.enum, # error: enum only takes a single argument
    c: T.enum(1),
    d: T.enum([]), # error: enum([]) is invalid
    e: T.enum([meth]), # error: Unsupported type literal
    f: 0, # error: Unsupported type syntax
    g: T.any(*[1,2]),
    h: T.junk, # error: Unsupported method T.junk
    i: T.class_of(T1, T2), # error: class_of only takes a single argument
    j: T.class_of(T.nilable(Integer)), # error: T.class_of needs a Class as its argument
    k: T.class_of(1), # error: T.class_of needs a Class as its argument
    l: {[] => String}, # error: Shape keys must be literals
    m: {foo: 0} # error: Unsupported type syntax
  )
  .returns(T2)
  def bad(a, b, c, d, e, f, g, h, i, j, k, l, m)
  end
end
